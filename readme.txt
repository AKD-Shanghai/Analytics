1.将本文件夹下的mode文件夹拷贝到任意位置,该路径记为 path_mode
2.修改TX1ProcessFlow.xml文件内容，修改model_dir属性值。
文件中所有节点mode_dir值修改为path_mode，除FACEDetection节点的的mode_dir为path_mode/MTmodel
3.将修改后的TX1ProcessFlow.xml拷贝到/etc目录下
4.VideoAnalyticsApi.hpp为头文件
