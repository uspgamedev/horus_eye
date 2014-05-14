music = "musics/Arabesque.ogg"
width = 20 
height = 60
rooms = { { 0, 0, "stage" }}
start_position = { "stage", 10, 2 }
stage = {
  width = 20,
  height = 60,
  matrix = [[
#########E##########
%........@.........#
%..................#
%..................#
%..................#
%......N.N.N.......#
%..................#
%..................#
%......%%%%%.......#
%...G..............#
%..................#
%..................#
%%%%%%%%....########
       %....#       
       %....#       
       %....#       
       %%..##       
        %..#        
        %..#        
        %..#        
        %..#        
        %MM#        
        %MM#        
        %MM#        
        %MM#        
        %MM#        
        %MM#        
        %MM#        
        %MM#        
        %MM#        
        %BB#        
        %BB#        
        %BB#        
        %..#        
        %..#        
        %..#        
        %..#        
        %..#        
        %..#        
        %..#        
      %%%..###      
      %......#      
      %......#      
      %......#      
######%......#######
%..................#
%..................#
%..................#
%..................#
%..................#
%..................#
%........LN........#
%..................#
%..................#
%........DD...U....#
%..................#
%..................#
%..................#
%..................#
%%%%%%%%%%%%%%%%%%%#
]]
}

oldmatrix = stage.matrix
stage.matrix = ""
for line in oldmatrix:gmatch("[^\n]+") do stage.matrix = line .. "\n" .. stage.matrix end
start_position[3] = height - start_position[3]

function new()
  local level = {}
  function level:Start(native, campaign)
    campaign:LevelStart(native, start_position)
  end
  function level:End(native, campaign) end
  return level
end
