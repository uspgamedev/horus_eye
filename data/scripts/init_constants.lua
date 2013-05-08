require "constants"

constants.RegisterDouble("PLAYER_MOUSE_HEIGHT_OFFSET", 58.0)

constants.RegisterInt("PROJECTILE_SPRITE_HEIGHT", 32)
constants.RegisterDouble("PROJECTILE_HEIGHT", 12.0)

constants.RegisterDouble("MAGICMISSILE_DURATION",   4.0)
constants.RegisterDouble("MAGICMISSILE_DAMAGE",   200.0)
constants.RegisterDouble("MAGICMISSILE_SPEED",     20.0)

constants.RegisterDouble("MAGICBALL_DURATION",   4.0)
constants.RegisterDouble("MAGICBALL_DAMAGE",   200.0)
constants.RegisterDouble("MAGICBALL_SPEED",      8.0)

constants.RegisterDouble("MUMMYPROJECTILE_DURATION",   3.5)
constants.RegisterDouble("MUMMYPROJECTILE_SPEED", 8.0)


constants.RegisterDouble("FIREBALL_DURATION",     3.0)
constants.RegisterDouble("LIGHTNING_DURATION",    3.5)


constants.RegisterInt("LIFEPOTION_RECOVER_LIFE", 200)
constants.RegisterInt("MANAPOTION_RECOVER_MANA", 200)

constants.RegisterInt("HERO_WIDTH", 110)
constants.RegisterInt("HERO_HEIGHT", 110)
constants.RegisterInt("HERO_MAX_WEAPONS", 6)
constants.RegisterInt("HERO_MIN_MANA_BLOCKS", 1)
constants.RegisterInt("HERO_MAX_MANA_BLOCKS", 5)
constants.RegisterInt("HERO_BASE_MANA_REGEN_RATIO", 3)

constants.RegisterInt("SIGHT_POTION_MAX_STACK", 5)

constants.RegisterInt("LIFE_BAR_OFFSET_X", 36)
constants.RegisterInt("LIFE_BAR_OFFSET_Y", 100)
constants.RegisterInt("MANA_BAR_OFFSET_X", 36)
constants.RegisterInt("MANA_BAR_OFFSET_Y", 100)
constants.RegisterInt("TOTEM_OFFSET_X", 36)
constants.RegisterInt("TOTEM_OFFSET_Y", 80)

constants.RegisterInt("MUMMY_LIFE", 400)
constants.RegisterInt("MUMMY_DAMAGE", 200)

constants.RegisterInt("RANGED_MUMMY_LIFE", 400)
constants.RegisterInt("RANGED_MUMMY_DAMAGE", 200)

constants.RegisterInt("BIG_MUMMY_LIFE", 1200)
constants.RegisterInt("BIG_MUMMY_DAMAGE", 300)

constants.RegisterInt("PAPER_MUMMY_LIFE", 1)
constants.RegisterInt("PAPER_MUMMY_DAMAGE", 25)

constants.RegisterInt("FIREBALL_EXPLOSION_DAMAGE", 200)
constants.RegisterInt("FIREBALL_COST", 200)
constants.RegisterInt("QUAKE_EXPLOSION_DAMAGE", 400)
constants.RegisterInt("QUAKE_COST", 100)
constants.RegisterInt("QUAKE_BLOCK_COST", 1)
constants.RegisterDouble("LIGHTNING_DAMAGE", 150)
constants.RegisterInt("LIGHTNING_COST", 100)
constants.RegisterInt("METEOR_EXPLOSION_DAMAGE", 500)
constants.RegisterInt("METEOR_COST", 200)
constants.RegisterInt("METEOR_BLOCK_COST", 1)

constants.RegisterInt("PHARAOH_LIFE", 6000)
constants.RegisterInt("PHARAOH_MANA", 200)
constants.RegisterInt("PHARAOH_SUMMON_MANA_COST", 60)
constants.RegisterInt("PHARAOH_DAMAGE", 400)
constants.RegisterInt("PHARAOH_RANGED_DAMAGE", 200)

constants.RegisterDouble("DOOR_HOTSPOT_WIDTH", 153.0/2.0)
constants.RegisterDouble("DOOR_HOTSPOT_HEIGHT", 109.0-(91.0/2.0))
constants.RegisterDouble("DOOR_BOUND_WIDTH", 2.0)
constants.RegisterDouble("DOOR_BOUND_HEIGHT", 1.0)

constants.RegisterDouble("CONDITION_DURATION", 30.0)

constants.RegisterDouble("FLOOR_WIDTH", 106.0)
constants.RegisterDouble("FLOOR_HEIGHT", 54.0)
constants.RegisterDouble("FLOOR_HOTSPOT_X", 106.0/2.0)
constants.RegisterDouble("FLOOR_HOTSPOT_Y", 54.0/2.0)

constants.RegisterDouble("WALL_HOTSPOT_X", 106.0/2.0)
constants.RegisterDouble("WALL_HOTSPOT_Y", 157.0)

constants.RegisterDouble("LIFE_BAR_WIDTH", 22.0)
constants.RegisterDouble("LIFE_BAR_HEIGHT", 456.0)
constants.RegisterDouble("MANA_BAR_WIDTH", 22.0)
constants.RegisterDouble("MANA_BAR_HEIGHT", 465.0)
constants.RegisterDouble("TOTEM_WIDTH", 72.0)
constants.RegisterDouble("TOTEM_HEIGHT", 518.0)

constants.RegisterDouble("LIGHT_RADIUS_THRESHOLD", 0.25)
constants.RegisterDouble("LIGHT_RADIUS_INITIAL", 4.0)

constants.RegisterDouble("HERO_SPEED", 4.0)
constants.RegisterDouble("HERO_MAX_LIFE", 1000.0)
constants.RegisterDouble("HERO_MANA_PER_BLOCK", 200.0)
constants.RegisterDouble("HERO_MANA_REGEN_BASE", 20.0)

constants.RegisterDouble("LIGHTNING_SPEED", 24.0)
constants.RegisterDouble("LIGHTNING_HEIGHT", 12.0)

constants.RegisterDouble("FIREBALL_SPEED", 10.0)
constants.RegisterDouble("FIREBALL_HEIGHT", 12.0)

constants.RegisterDouble("EXPLOSION_SPEED", 8.0)
constants.RegisterDouble("EXPLOSION_WIDTH", 256.0)
constants.RegisterDouble("EXPLOSION_HEIGHT", 128.0)

constants.RegisterDouble("FIREBALL_EXPLOSION_RADIUS", 1.6)
constants.RegisterDouble("QUAKE_EXPLOSION_RADIUS", 1.25)
constants.RegisterDouble("QUAKE_EXPLOSION_RANGE", 6.0)
constants.RegisterDouble("METEOR_EXPLOSION_RADIUS", 2.0)

constants.RegisterDouble("SANDSTORM_FADEOUT_TIME", 0.50)
constants.RegisterDouble("SANDSTORM_PROJECTILE_INTERVAL", 0.125)
constants.RegisterDouble("SANDSTORM_COST", 200.0)
constants.RegisterDouble("SANDSTORM_MAINTAIN_COST", 75.0)
constants.RegisterDouble("SANDSTORM_ANGLE", math.pi/6.0)
constants.RegisterDouble("SANDSTORM_LENGHT", 2.5)

constants.RegisterDouble("SIGHT_POTION_INCREASE", 1.2)

constants.RegisterDouble("MUMMY_SIGHT_RANGE", 20.0)

constants.RegisterDouble("MUMMY_RADIUS", 0.3)
constants.RegisterDouble("MUMMY_SPEED", 2.9)

constants.RegisterDouble("RANGED_MUMMY_RANGE", 5.0)

constants.RegisterDouble("BIG_MUMMY_RADIUS", 0.5)
constants.RegisterDouble("BIG_MUMMY_SPEED", 2.0)


constants.RegisterDouble("PHARAOH_RADIUS", 0.45)
constants.RegisterDouble("PHARAOH_SPEED", 3.3)
constants.RegisterDouble("PHARAOH_SUMMON_RANGE", 2.0)
constants.RegisterDouble("PHARAOH_MANA_REGEN", 5.0)

constants.RegisterDouble("POTION_SPRITE_CENTER_X", 15.0)
constants.RegisterDouble("POTION_SPRITE_CENTER_Y", 15.0)
constants.RegisterDouble("POTION_HEIGHT", 16.0)
