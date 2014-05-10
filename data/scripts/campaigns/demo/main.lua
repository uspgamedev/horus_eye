function new(native)
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
  
  function campaign:Focus(native)
  
    if self.hero_died then
      -- Show 'You Died' image.
      native:Finish()
  
    elseif not self.next_level then
      self.next_level = 1
      context.ShowScrollingText "Intro"
      
    elseif self.level_list[self.next_level] then
      native:LoadLevel(self.level_list[self.next_level])
      self.next_level = self.next_level + 1
      
    else
      native:Finish()
      
    end
  
  end
  
  function campaign:DeFocus(native) end
  function campaign:End(native) end
  
  return campaign
end