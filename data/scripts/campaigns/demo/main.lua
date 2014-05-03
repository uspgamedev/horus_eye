function new()
  return {
    level_list = { "levels.demo_lvl_1", "levels.demo_lvl_2" },
    on_start = function(self, campaign)
      print("SUP GALS")
      self.next_level = 1
    end,
    on_focus = function(self, campaign)
      if self.level_list[self.next_level] then
        campaign:LoadLevel(self.level_list[self.next_level])
        self.next_level = self.next_level + 1
      else
        campaign:Finish()
      end
    end,
  }
end