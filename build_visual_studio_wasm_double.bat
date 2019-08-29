IF NOT EXIST bin mkdir bin


rem you can also override and hardcode the Python path like this (just remove the # hashmark in next line)
rem SET myvar=c:\python-3.5.2

cd build3


premake4  --double  --targetdir="../bin" vs2010 

rem premake4  --double   --enable_multithreading --midi --enable_static_vr_plugin --enable_openvr --enable_pybullet --python_include_dir="%myvar%/include" --python_lib_dir="%myvar%/libs"   --targetdir="../binserver" vs2010 
rem premake4  --double --enable_grpc --enable_multithreading --midi --enable_static_vr_plugin --enable_openvr --enable_pybullet --python_include_dir="%myvar%/include" --python_lib_dir="%myvar%/libs"   --targetdir="../binserver" vs2010 
rem premake4  --serial --audio --double --midi --enable_openvr --enable_pybullet --python_include_dir="%myvar%/include" --python_lib_dir="%myvar%/libs"   --targetdir="../bin" vs2010 

start vs2010

