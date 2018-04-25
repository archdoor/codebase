1. 下载并解压：
https://github.com/downloads/HardySimpson/zlog/zlog-latest-stable.tar.gz
tar -zxvf zlog-latest-stable.tar.gz
cd zlog-x.x.x/

2. 编译安装：
make
sudo make install


可能出现的问题：
程序连接时报错： error while loading shared libraries: libzlog.so.1.1:

需要安装包lib32z1：sudo apt-get install lib32z1 

