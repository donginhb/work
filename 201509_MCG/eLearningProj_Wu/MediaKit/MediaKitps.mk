
MediaKitps.dll: dlldata.obj MediaKit_p.obj MediaKit_i.obj
	link /dll /out:MediaKitps.dll /def:MediaKitps.def /entry:DllMain dlldata.obj MediaKit_p.obj MediaKit_i.obj \
		kernel32.lib rpcndr.lib rpcns4.lib rpcrt4.lib oleaut32.lib uuid.lib \

.c.obj:
	cl /c /Ox /DWIN32 /D_WIN32_WINNT=0x0400 /DREGISTER_PROXY_DLL \
		$<

clean:
	@del MediaKitps.dll
	@del MediaKitps.lib
	@del MediaKitps.exp
	@del dlldata.obj
	@del MediaKit_p.obj
	@del MediaKit_i.obj
