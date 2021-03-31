# Neocron Log Parser
Parses Neocron's log file and displays it's damage information.

## ToDo
- [ ] Toggle: Auto-reload file (pause, continue)
- [ ] Toggle: Auto-select newest entry
- [ ] Export as CSV
- [ ] Filter duplicates/matching damage entries (Salve shot of three rounds have three times the same information)

__Full damage entry, Heal?__
This one is skipped, because the damage value is negative.
```
Local Player:Damage() - Damage processing statistics!
Damage: -11.183 Target Fulldamage HitZone 1 - Part 0
Results of this target: Damage -11.183 (Reduction: 0.000 - -0.000 Percentage) - ResistanceCap: 0.840!
```

