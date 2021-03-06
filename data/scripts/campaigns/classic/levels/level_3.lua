music = "musics/Arabesque.ogg"
width = 36 
height = 50
rooms = { { 0, 0, "stage" }}
start_position = { "stage", 4, 2 }
stage = {
width = 36 ,
height = 50,
  matrix = [[
###E#######   #############         
%..@......#   %...........#         
%.........#   %...........#         
%.........####%...........#         
%....S....................#         
%.........................#         
%.........%%%%%...........#         
%.........#   %...........#         
%%%%%%%%%%#   %...........#         
              %M.M.B.B.M.M#         
              %...........#         
              %...........#         
              %.....S.....#         
              %...........#         
              %...........#         
              %..R..R..R..#         
              %...........#         
              %%%%%..%%%%%#         
                  %..#              
                  %..#              
                  %..#              
                  %..#              
               ###%..####           
               %........#           
           ####%........#####       
      #####%................######  
      %R...M................M...R#  
      %...M..................M...#  
      %L.B.........S..........B.N#  
      %...M..................M...#  
      %R...M................M...R#  
      %%%%%%................%%%%%#  
           %%%%%........%%%%%       
               %........%           
               %%%%..%%%%           
                  %..#              
                  %..#              
##########        %..#              
%........#        %..#              
%..NL....#       #%..#              
%........#########...#              
%....................#              
%..DD...............%#              
%........%%%%%%%%%%%%               
%...U....#                          
%........#                          
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
