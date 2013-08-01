music = "musics/running_from_something.ogg"
width = 25 
height = 55
rooms = { { 0, 0, "stage" }}
start_position = { "stage", 17, 51 }
stage = {
  width = 25,
  height = 55,
  matrix = [[
#########################
%.......................#
%...........DD..U.......#
%..%%%.............%%%..#
%....%.............%....#
%..L.%.............%.L..#
%....%....L.P.L....%....#
%..%%%....N...N....%%%..#
%.......................#
%.......................#
%.......................#
%..%%%.............%%%..#
%....%.............%....#
%..N.%.............%.N..#
%....%.............%....#
%..%%%.............%%%..#
%.......................#
%.......................#
%..%%%.............%%%..#
%....%.............%....#
%..L.%.............%.L..#
%....%.............%....#
%..%%%.............%%%..#
%.......................#
%.......................#
%%%%%%%%%%%.....%%%%%%%%#
          %.....#        
          %.....#        
          %.....#        
          %.....#        
          %.....#        
          %.....#        
          %.....#        
          %.....#        
          %.....#        
          %.....#        
          %.....#        
          %.....#        
          %.....#        
          %.....#        
          %.....#        
          %.....#        
          %.....#        
          %.....#        
          %.....#        
          %.....#        
          %.....#        
          %.....#        
          %.....#        
        %%%.....###      
        %L.......N#      
        %L.......@E      
        %L.......N#      
        %%%.....###      
          %.SSS.#        
          %%%%%%#        
]]
}

oldmatrix = stage.matrix
stage.matrix = ""
for line in oldmatrix:gmatch("[^\n]+") do stage.matrix = line .. "\n" .. stage.matrix end
start_position[3] = height - start_position[3]