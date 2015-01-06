
require 'builder'

function new(native)

  require 'assetloader'
  assetloader.LoadAssets()

  local campaign = {
    level_list = {
      "level_1", 
      "level_lightningtest", 
      "level_2", 
      "level_3", 
      "level_4", 
      "level_turtle", 
      "throne_level",
    },
  }
  
  function campaign:create_hero_death_callback()
    return function(wobj)
      self.hero_died = true
      local level = wobj:current_room():level()
      level:SetHero(component.WObjPtr())
      level:Finish()
    end
  end

  function campaign:Start(native)
    self.next_level = 1
    context.ShowScrollingText "Intro"
  end

  function campaign:OnSceneFinished(native)
    if self.hero_died then
      -- TODO: Show 'You Died' image.
      native:End()

    elseif self.level_list[self.next_level] then
      native:LoadLevel("levels." .. self.level_list[self.next_level])
      self.next_level = self.next_level + 1

    else
      -- YOU WIN!
      native:End()
    end
  end

  function campaign:End(native) end
  
  function campaign:LevelStart(level_native, start_data)
    local hero = builder.Kha()
    hero:AddOnRemoveCallback(self:create_hero_death_callback())
    level_native:SetHero(hero)
    
    local initial_room = level_native:GetRoom(start_data[1])
    initial_room:AddObject(hero, ugdk_math.Vector2D(start_data[2], start_data[3]), true)
    level_native:ChangeFocusedRoom(initial_room)
    
    -- Don't hold these shared_ptr in scripts...
    hero = nil
    collectgarbage()
  end
  
  return campaign
end