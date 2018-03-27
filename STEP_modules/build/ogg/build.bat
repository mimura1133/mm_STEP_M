chcp 932

if not exist "libogg_static.vcxproj" (
	devenv libogg_static.sln /upgrade
)

msbuild libogg_static.sln /p:WindowsTargetPlatformVersion=%TargetPlatformVersion% /p:Configuration=%Configuration%;Platform=%Platform%
