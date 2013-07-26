music = "musics/Arabesque.ogg"
width = 11
height = 34
rooms = { { 0, 0, "stage" }}
start_position = { "stage", 9, 2 }
stage = {
  width = 11,
  height = 34,
  matrix = [[
#########E#
%........@#
%.........#
%.........#
%.........#
%.........#
%.........#
%.........#
%...%%%%%%#
%...#      
%...#      
%...#######
%........N#
%.........#
%.......M.#
%.........#
%.........#
%.........#
%.........#
%%%%%%%...#
      %...#
      %...#
######%...#
%.........#
%..L......#
%..M......#
%..M......#
%..M......#
%....U....#
%.........#
%....DD...#
%.L.......#
%.........#
%%%%%%%%%%#
]]
}

oldmatrix = stage.matrix
stage.matrix = ""
for line in oldmatrix:gmatch("[^\n]+") do stage.matrix = line .. "\n" .. stage.matrix end
start_position[3] = height - start_position[3]