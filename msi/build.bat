candle.exe -nologo jntetri.wxs -ext WixUtilExtension
light.exe -nologo -out JNTETRI.msi -ext WixUtilExtension -ext WixUIExtension jntetri.wixobj
pause