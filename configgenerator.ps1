if((Test-Path src\game\config.h) -And (((Get-Item src\game\config.h.in).LastWriteTime - (Get-Item src\game\config.h).LastWriteTime).TotalMinutes -gt 0)) {
	Remove-Item src\game\config.h
}
if(((Test-Path src\game\config.h) -eq $False) -And (Test-Path src\game\config.h.in)) {
	Get-Content src\game\config.h.in | ForEach-Object { $_ -replace "@GAME_VERSION@", "1.1.1 Alpha" -replace "@GAME_LOCAL_DIR@", "./data" -replace "@CHECKMAC@", "" } | Set-Content src\game\config.h
}