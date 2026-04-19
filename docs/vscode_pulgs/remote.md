# VSCode 配置远程开发环境

VSCode Remote-SSH扩展允许在任何远程计算机、虚拟机或具有正在运行的 SSH 服务器的容器上打开远程文件夹并充分利用 VSCode 的功能集。连接到服务器后，可以与远程文件系统上任何位置的文件和文件夹进行交互,本地计算机上不需要有源代码，因为扩展直接在远程计算机上运行命令和其他扩展    

1、安装 Remote-SSH 插件    

2、打开 Remote-SSH 配置文件 或 直接配置 .ssh/config 文件    
  ```bash
  vi .ssh/config
  
  Host 1.92.89.83
    HostName 1.92.89.83
    Port 22
    User brightl
    ForwardAgent yes
  ```

3、配置 SSH 免密登陆    
  ```bash
  # 本地客户端生成公私钥 
  ssh-keygen -t rsa -C 'brightl3016@outlook.com' -f ~/.ssh/remote_ssh_id_rsa
  # 这条命令会在用户目录.ssh下创建公私钥 remote_ssh_id_rsa、remote_ssh_id_rsa.pub 文件

  # 本地客户端上传公钥到远程服务器 
  ssh-copy-id -i ~/.ssh/remote_ssh_id_rsa.pub brightl@1.92.89.83
  # 这条命令是将本地客户端 remote_ssh_id_rsa.pub 公钥内容写到服务器上的 .ssh 目录 authorized_keys 文件里了

  # 本地客户端配置 .ssh/config 文件
  vi .ssh/config

  # remote_ssh
  Host remote_ssh
    HostName 1.92.89.83
    User brightl
    Port 22
    PreferredAuthentications publickey
    IdentityFile ~/.ssh/remote_ssh_id_rsa
    User brightl3016@outlook.com

  # 测试免密登陆 
  ssh remote_ssh 

  # 上传文件和目录 
  scp /Users/minghui.liu/Downloads/codelldb-linux-x64.vsix remote_ssh:/home/brightl/conf/
  scp -r /path/to/local/directory remote_ssh:/remote/path/ 
  ```