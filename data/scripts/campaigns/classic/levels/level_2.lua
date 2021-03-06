music = "musics/Arabesque.ogg"
width = 31
height = 30
rooms = { { 0, 0, "stage" }}
start_position = { "stage", 5, 2 }
stage = {
width = 31,
height = 30,
  matrix = [[
####E####                      
%...@...#######                
%.............#                
%.............#                
%.............#                
%.......%%%%..##########       
%%%%%%%%%  %..#.......S#       
           %SS#..LN..###       
#########  %..#.R..R.#         
%.......###%..#......#         
%R............#......#         
%.............#.B..B.#         
%%%%%...%%%%%%#......#         
%.......#     #......#         
%R......#     #......#         
%.......#     #......#         
%%%%%...#######......######### 
%............................# 
%R...........................# 
%.......%%%%%%%......%%%%%%..# 
%%%%%%%%%     %%%%%%%%    %..# 
                          %..# 
                     %%%%%%..##
                     %M......M#
                     %........#
                     %..ULN...#
                     %........#
                     %...DD...#
                     %M......M#
                     %%%%%%%%%#
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
