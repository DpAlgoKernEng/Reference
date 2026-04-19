## linux 环境配置

1、虚拟机、linux系统、云服务器搭建
  ```bash     
  # 内存、CPU、磁盘、网络配置    
  # linux磁盘分区、文件格式配置    
  # linux目录结构    
  # 用户命令 df列出文件系统的整体磁盘使用量、du检查磁盘空间使用量、fdisk用于磁盘分区    
  # 用户命令 mkfs文件系统的格式化、fsck磁盘检验、mount磁盘挂载、umount磁盘卸载    
  # 用户命令 ls、cd、pwd、mkdir、rmdir、cp、rm、mv #处理目录文件    
  # 用户命令 cat、tac、nl、more、less、head、tail #查看文件内容   
  # 用户命令 chown、chmod、chgrp #处理文件基本属性   
  ```

2、用户和用户组添加 
  ```bash  
  man adduser、man usermod 查看命令介绍文档    
  sudo adduser brightl #添加新用户brightl   
  sudo usermod -aG sudo brightl #将brightl用户追加到sudo组使其具有管理员权限    
  id brightl、groups brightl #查看用户所在组信息   
  cat /etc/passwd #查看系统的用户账户信息    
  cat /etc/shadow #查看系统中用户密码和其他安全相关信息   
  cat /etc/group #查看系统中的用户组信息   
  # 用户命令 useradd、adduser、usermod、userdel、passwd   
  # 用户组命令 groupadd、groupdel、groupmod、newgrp   
  ```

3、普通用户免密sudo权限授权
  ```bash   
  su - #切换到root用户   
  chmod u+w /etc/sudoers #增加文件的写权限   
  visudo 或 vim /etc/sudoers #打开配置sudo命令权限的文件   
  brightl ALL=(ALL) NOPASSWD: ALL #添加sudo命令免密授权、必须添加在最后放防止被后面group的设置覆盖了   
  chmod u-w /etc/sudoers #移除文件的写权限   
  ```

4、允许root用户远程登录登陆
  ```bash   
  sudo vi /etc/ssh/sshd_config #用管理员权限打开ssh服务配置文件   
  PermitRootLogin yes #允许以root用户直接通过ssh登录、允许其远程登录   
  service sshd restart 或 systemctl restart sshd #重新启动ssh服务、配置生效   
  ```

5、ssh 登陆
  ```bash   
  ssh root@1.92.89.83   
  ssh brightl@1.92.89.83    
  ```  

6、代码文件目录创建   
  ```bash 
  pwd  
  mkdir brightl_code
  mkdir conf   
  ls -al brightl_code
  ``` 

7、上传文件和目录   
  ```bash
  scp /Users/minghui.liu/Downloads/codelldb-linux-x64.vsix brightl@1.92.89.83:/home/brightl/conf/
  scp -r /path/to/local/directory brightl@1.92.89.83:/remote/path/
  ```    
   
