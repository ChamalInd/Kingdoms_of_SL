# ============================================================================
# Kingdoms of SL - demo recording (https://github.com/charmbracelet/vhs)
#
# Before recording:
#   1. make                      (build the simulation: creates ./kingdoms)
#   2. brew install vhs          (also needs ttyd and ffmpeg; see the VHS README)
#
# Record:
#   vhs demo.tape                (creates demo.gif next to this file)
# ============================================================================

Output demo.gif

# --- Look and feel ----------------------------------------------------------
Set Shell "bash"
Set FontSize 14
Set Width 1000
Set Height 760
Set Padding 20
Set Theme "Catppuccin Mocha"
Set TypingSpeed 45ms
Set Framerate 24

Show

# --- Scene 1: title and the four kingdoms -----------------------------------
Type "# Kingdoms of SL - an autonomous strategy simulation in C++"
Enter
Sleep 1s
Type "head -n 19 output.log"
Enter
Sleep 6s
Ctrl+L
Sleep 500ms

# --- Scene 2: one ruler's first turn (claim, build, recruit, march) ---------
Type "# Round 01 of The Conqueror's (Anuradhapura) turn"
Enter
Sleep 1s
Type "sed -n '56,72p' output.log"
Enter
Sleep 5s
Ctrl+L
Sleep 500ms

# --- Scene 3: a battle ------------------------------------------------------
Type "# Later on, armies clash"
Enter
Sleep 1s
Type "sed -n '6338,6353p' output.log"
Enter
Sleep 6s
Ctrl+L
Sleep 500ms

# --- Scene 4: round summary -------------------------------------------------
Type "# Every 10 rounds: a status report and a random world event"
Enter
Sleep 1s
Type 'grep -m1 -A 28 "Round 9 Summary" output.log'
Enter
Sleep 5s
Ctrl+L
Sleep 500ms

# --- Scene 5: the result ----------------------------------------------------
Type "# ...and after up to 200 rounds, a winner"
Enter
Sleep 1s
Type "tail -n 8 output.log"
Enter
Sleep 5s
