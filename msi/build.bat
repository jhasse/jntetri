SET DataDir=..\data
"%WIX%bin\heat.exe" dir "%DataDir%" -cg DataFiles -var env.DataDir -gg -scom -sreg -sfrag -srd -dr data -out data.wxs || goto :error
"%WIX%bin\candle.exe" jntetri.wxs -ext WixUtilExtension || goto :error
"%WIX%bin\candle.exe" data.wxs -ext WixUtilExtension || goto :error
"%WIX%bin\light.exe" -out JNTETRI.msi -ext WixUtilExtension -ext WixUIExtension jntetri.wixobj data.wixobj || goto :error

:error
