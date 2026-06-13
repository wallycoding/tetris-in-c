#!/usr/bin/env python3
"""
Gera a trilha e os efeitos sonoros 8-bit do Tetris por sintese de onda.

Saida (em ./assets):
  music.wav      -> trilha chiptune em loop (tema "Korobeiniki", dominio publico)
  move.wav       -> mover a peca (esquerda/direita)
  rotate.wav     -> girar a peca
  drop.wav       -> peca travar/cair no lugar
  lineclear.wav  -> completar uma linha

Sem dependencias alem de numpy. PCM 16-bit mono, 44.1kHz.
"""

import os
import wave
import numpy as np

SR = 44100  # sample rate
OUT_DIR = os.path.join(os.path.dirname(os.path.dirname(os.path.abspath(__file__))), "assets")

NOTE_BASE = {
    "C": 0, "C#": 1, "Db": 1, "D": 2, "D#": 3, "Eb": 3, "E": 4, "F": 5,
    "F#": 6, "Gb": 6, "G": 7, "G#": 8, "Ab": 8, "A": 9, "A#": 10, "Bb": 10, "B": 11,
}


def freq(note):
    """Converte um nome de nota (ex: 'A4', 'C#5', 'R'=pausa) em frequencia (Hz)."""
    if note in (None, "R"):
        return 0.0
    name, octave = note[:-1], int(note[-1])
    midi = NOTE_BASE[name] + (octave + 1) * 12  # C4 = 60
    return 440.0 * 2 ** ((midi - 69) / 12)


def square(f, t, duty=0.5):
    if f == 0.0:
        return np.zeros_like(t)
    return np.where((f * t) % 1.0 < duty, 1.0, -1.0)


def triangle(f, t):
    if f == 0.0:
        return np.zeros_like(t)
    return 4.0 * np.abs(((f * t) % 1.0) - 0.5) - 1.0


def envelope(n, attack=0.008, release=0.04):
    """Rampas de entrada/saida pra evitar estalos (clicks)."""
    env = np.ones(n)
    a = min(int(SR * attack), n // 2)
    r = min(int(SR * release), n // 2)
    if a > 0:
        env[:a] = np.linspace(0.0, 1.0, a)
    if r > 0:
        env[-r:] = np.linspace(1.0, 0.0, r)
    return env


def render_note(note, dur, wave_kind="square", duty=0.5, vol=0.3,
                attack=0.008, release=0.04):
    n = int(SR * dur)
    t = np.arange(n) / SR
    f = freq(note)
    if f == 0.0:
        return np.zeros(n)
    w = square(f, t, duty) if wave_kind == "square" else triangle(f, t)
    return w * envelope(n, attack, release) * vol


def bitcrush(x, levels=16):
    """Quantiza a amplitude pra dar aquela aspereza de 8-bit."""
    return np.round(x * levels) / levels


def write_wav(path, samples):
    samples = np.clip(samples, -1.0, 1.0)
    pcm = (samples * 32767).astype("<i2")
    with wave.open(path, "wb") as w:
        w.setnchannels(1)
        w.setsampwidth(2)
        w.setframerate(SR)
        w.writeframes(pcm.tobytes())
    print(f"  {os.path.relpath(path)}  ({len(samples)/SR:.2f}s)")


# ---------------------------------------------------------------------------
# Trilha: composicao original "Krasny Blok" - estilo folk russo (kalinka/troika)
# La menor HARMONICA (Sol# como sensivel -> 2a aumentada F->G#, o "tempero" russo)
# alternancia i-V (Am-E) constante e baixo "oom-pah". 16 compassos em loop.
# ---------------------------------------------------------------------------

BPM = 158
BEAT = 60.0 / BPM  # duracao de uma seminima (quarter note) em segundos

# Harmonia de cada compasso: (raiz do baixo, notas do acorde p/ o arpejo).
# O acorde de E carrega o G#4 (sensivel) que da o brilho russo.
CHORDS = [
    ("A", ["A4", "C5", "E5"]),    # 1  Am
    ("E", ["E4", "G#4", "B4"]),   # 2  E
    ("A", ["A4", "C5", "E5"]),    # 3  Am
    ("E", ["E4", "G#4", "B4"]),   # 4  E
    ("A", ["A4", "C5", "E5"]),    # 5  Am
    ("D", ["D5", "F5", "A5"]),    # 6  Dm
    ("E", ["E4", "G#4", "B4"]),   # 7  E
    ("A", ["A4", "C5", "E5"]),    # 8  Am
    ("A", ["A4", "C5", "E5"]),    # 9  Am
    ("E", ["E4", "G#4", "B4"]),   # 10 E
    ("A", ["A4", "C5", "E5"]),    # 11 Am
    ("E", ["E4", "G#4", "B4"]),   # 12 E
    ("D", ["D5", "F5", "A5"]),    # 13 Dm
    ("A", ["A4", "C5", "E5"]),    # 14 Am
    ("E", ["E4", "G#4", "B4"]),   # 15 E
    ("A", ["A4", "C5", "E5"]),    # 16 Am
]

# Melodia principal: (nota, duracao em batidas). Cada compasso soma 4 batidas.
# Usa G# (sensivel) e o salto F->G# pra evocar a cor da musica russa.
MELODY = [
    ("E5", .5), ("A5", .5), ("G#5", .5), ("A5", .5), ("E5", .5), ("C5", .5), ("A4", 1),  # 1
    ("B4", .5), ("C5", .5), ("D5", .5), ("E5", .5), ("G#5", 1), ("E5", 1),               # 2
    ("A5", .5), ("G#5", .5), ("A5", .5), ("B4", .5), ("C5", 1), ("A4", 1),               # 3
    ("E5", .5), ("D5", .5), ("C5", .5), ("B4", .5), ("G#4", 1), ("E4", 1),               # 4
    ("A4", .5), ("C5", .5), ("E5", .5), ("A5", .5), ("E5", .5), ("C5", .5), ("A4", 1),   # 5
    ("D5", .5), ("F5", .5), ("A5", .5), ("F5", .5), ("D5", 1), ("A4", 1),                # 6
    ("G#4", .5), ("B4", .5), ("E5", .5), ("G#5", .5), ("B5", .5), ("G#5", .5), ("E5", 1),# 7
    ("A5", 1), ("E5", 1), ("C5", 1), ("A4", 1),                                          # 8
    ("A4", .5), ("B4", .5), ("C5", .5), ("D5", .5), ("E5", .5), ("F5", .5), ("E5", 1),   # 9
    ("G#5", .5), ("F5", .5), ("E5", .5), ("D5", .5), ("B4", 1), ("G#4", 1),              # 10
    ("A4", .5), ("C5", .5), ("E5", .5), ("C5", .5), ("A5", .5), ("E5", .5), ("C5", 1),   # 11
    ("B4", .5), ("G#4", .5), ("E4", .5), ("G#4", .5), ("B4", 1), ("E5", 1),              # 12
    ("D5", .5), ("F5", .5), ("A5", .5), ("D6", .5), ("A5", 1), ("F5", 1),                # 13
    ("E5", .5), ("C5", .5), ("A4", .5), ("C5", .5), ("E5", 1), ("A5", 1),                # 14
    ("G#5", .5), ("B5", .5), ("E5", .5), ("G#5", .5), ("B4", 1), ("G#4", 1),             # 15
    ("A5", 1), ("G#5", .5), ("A5", .5), ("E5", 1), ("A4", 1),                            # 16
]


def build_lead():
    parts = [render_note(n, b * BEAT, "square", duty=0.5, vol=0.34,
                         release=0.03) for n, b in MELODY]
    return np.concatenate(parts)


def build_bass():
    parts = []
    eighth = 0.5 * BEAT
    for root, _ in CHORDS:
        for i in range(8):  # 8 colcheias por compasso, saltando oitavas
            octv = "2" if i % 2 == 0 else "3"
            parts.append(render_note(root + octv, eighth, "triangle",
                                     vol=0.32, release=0.02))
    return np.concatenate(parts)


def build_arp():
    parts = []
    sixteenth = 0.25 * BEAT
    pattern = [0, 1, 2, 1]  # sobe e desce pelos tons do acorde
    for _, tones in CHORDS:
        for i in range(16):  # 16 semicolcheias por compasso
            parts.append(render_note(tones[pattern[i % 4]], sixteenth,
                                     "square", duty=0.25, vol=0.13,
                                     attack=0.002, release=0.015))
    return np.concatenate(parts)


def build_music():
    lead, bass, arp = build_lead(), build_bass(), build_arp()
    n = max(len(lead), len(bass), len(arp))
    lead = np.pad(lead, (0, n - len(lead)))
    bass = np.pad(bass, (0, n - len(bass)))
    arp = np.pad(arp, (0, n - len(arp)))
    mix = lead + bass + arp
    mix /= np.max(np.abs(mix)) + 1e-9
    mix *= 0.85
    return bitcrush(mix, levels=32)


# ---------------------------------------------------------------------------
# Efeitos sonoros
# ---------------------------------------------------------------------------

def sweep(f0, f1, dur, wave_kind="square", duty=0.5, vol=0.5,
          attack=0.002, release=0.03):
    n = int(SR * dur)
    t = np.arange(n) / SR
    f = np.linspace(f0, f1, n)
    phase = np.cumsum(f) / SR
    if wave_kind == "square":
        w = np.where((phase % 1.0) < duty, 1.0, -1.0)
    else:
        w = 4.0 * np.abs((phase % 1.0) - 0.5) - 1.0
    return w * envelope(n, attack, release) * vol


def sfx_move():
    return bitcrush(sweep(700, 480, 0.05, vol=0.45, duty=0.5))


def sfx_rotate():
    a = render_note("A4", 0.04, "square", duty=0.5, vol=0.45)
    b = render_note("E5", 0.06, "square", duty=0.5, vol=0.45)
    return bitcrush(np.concatenate([a, b]))


def sfx_drop():
    # corpo grave descendente + estalo de ruido = "tump"
    body = sweep(220, 70, 0.16, wave_kind="square", duty=0.5,
                 vol=0.5, release=0.08)
    n = len(body)
    noise = (np.random.RandomState(7).uniform(-1, 1, n)
             * np.linspace(0.5, 0.0, n) ** 2 * 0.35)
    return bitcrush(body + noise)


def sfx_lineclear():
    notes = [("C5", .07), ("E5", .07), ("G5", .07), ("C6", .12)]
    parts = [render_note(nm, d, "square", duty=0.5, vol=0.42,
                         attack=0.002, release=0.03) for nm, d in notes]
    return bitcrush(np.concatenate(parts))


def main():
    os.makedirs(OUT_DIR, exist_ok=True)
    print("Gerando audios 8-bit em ./assets ...")
    write_wav(os.path.join(OUT_DIR, "music.wav"), build_music())
    write_wav(os.path.join(OUT_DIR, "move.wav"), sfx_move())
    write_wav(os.path.join(OUT_DIR, "rotate.wav"), sfx_rotate())
    write_wav(os.path.join(OUT_DIR, "drop.wav"), sfx_drop())
    write_wav(os.path.join(OUT_DIR, "lineclear.wav"), sfx_lineclear())
    print("Pronto!")


if __name__ == "__main__":
    main()
