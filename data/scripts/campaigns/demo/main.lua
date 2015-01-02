function new(native)

  require 'assetloader'
  assetloader.LoadAssets()

  local campaign = {
    level_list = { "levels.demo_lvl_1", "levels.demo_lvl_2" },
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
      native:Finish()

    elseif self.level_list[self.next_level] then
      native:LoadLevel(self.level_list[self.next_level])
      self.next_level = self.next_level + 1

    else
      -- YOU WIN!
      native:Finish()
    end
  end

  function campaign:End(native) end
  
  return campaign
end