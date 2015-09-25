
MediaFileps.dll: dlldata.obj MediaFile_p.obj MediaFile_i.obj
	link /dll /out:MediaFileps.dll /def:MediaFileps.def /entry:DllMain dlldata.obj MediaFile_p.obj MediaFile_i.obj \
		kernel32.lib rpcndr.lib rpcns4.lib rpcrt4.lib oleaut32.lib uuid.lib \

.c.obj:
	cl /c /Ox /DWIN32 /D_WIN32_WINNT=0x0400 /DREGISTER_PROXY_DLL \
		$<

clean:
	@del MediaFileps.dll
	@del MediaFileps.lib
	@del MediaFileps.exp
	@del dlldata.obj
	@del MediaFile_p.obj
	@del MediaFile_i.obj
