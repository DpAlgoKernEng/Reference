## Git/Github 环境配置

该文档将介绍 Git/GitHub SSH 免密配置以及 Git 代码提交合并操作，Git 有两种克隆代码的方式：一种为 HTTP 另外一种为 SSH 免密登录。SSH 免密登录需要生成 SSH 密钥，然后把公钥关联到自己 GitHub 账户即可   

## 一、SSH 免密配置

### 1、添加用户邮箱
- 添加用户名邮箱   
  ```bash
  git config --global user.name "brightl"   
  git config --global user.email "brightl3016@outlook.com"   
  ```

### 2、生成 SSH 密钥
- 生成 SSH 密钥，创建 SSH key 执行命令下面命令，点击三次回车      
  ```bash
  ssh-keygen -t rsa -C 'brightl3016@outlook.com' -f ~/.ssh/gitee_id_rsa
  ssh-keygen -t rsa -C 'brightl3016@outlook.com' -f ~/.ssh/github_id_rsa
  ```
- 如果创建成功会在用户目录分别生成`.ssh`文件夹包含 github_id_rsa.pub(公钥文件)、github_id_rsa(私钥文件) 查看文件        
  ``` bash
  cat ~/.ssh/github_id_rsa.pub
  cat ~/.ssh/gitee_id_rsa.pub
  ```
- 在 ~/.ssh 目录下新建一个config文件，添加如下内容     
  ```bash
  # gitee_id_rsa
  Host gitee.com
    HostName gitee.com
    PreferredAuthentications publickey
    IdentityFile ~/.ssh/gitee_id_rsa
    User brightl3016@outlook.com

  # github                              # 
  Host github.com
    HostName github.com
    PreferredAuthentications publickey
    IdentityFile ~/.ssh/github_id_rsa
    User brightl3016@outlook.com

  # ssh.github.com
  Host github.com
      HostName ssh.github.com
      Port 443
      PreferredAuthentications publickey
      IdentityFile ~/.ssh/github_id_rsa
      User brightl3016@outlook.com    
  ```

### 3、GitHub 配置 SSH Key 公钥
- 注册并登录 GitHub，依次找到 Settings -> SSH and GPG keys   
- SSH key 中 新建一个 New SSH key   
- 打开 github_id_rsa.pub 文件，将生成的密钥完整的粘贴到 GitHub 中添加完成   

### 4、Git 测试 SSH 免密登录
- 远程仓库 push 东西的时候使用 SSH 连接就不需要登录了  
  ```bash
  ssh -T git@gitee.com
  ssh -T git@github.com
  ssh -T -p 443 git@ssh.github.com
  ```
# 这段文本是一个用于配置 SSH 的文件示例（通常是 ~/.ssh/config 文件），用于连接 GitHub 的两种不同方式。以下是详细解释：
# 第一部分：直接连接 GitHub
# github
Host github.com
    HostName github.com
    PreferredAuthentications publickey
    IdentityFile ~/.ssh/github_id_rsa
    User brightl3016@outlook.com
# github：注释，说明此部分配置是针对 GitHub 的
Host github.com：定义一个主机别名，后续的配置均是针对 github.com 的
HostName github.com：实际主机名，表示连接到 GitHub 的默认域名
PreferredAuthentications publickey：指定优先使用公钥认证进行连接
IdentityFile ~/.ssh/github_id_rsa：指定用于连接的私钥文件路径
User brightl3016@outlook.com：指定用于连接的用户名

# 第二部分：通过 ssh.github.com 连接 GitHub
# ssh.github.com
Host github.com
    HostName ssh.github.com
    Port 443
    PreferredAuthentications publickey
    IdentityFile ~/.ssh/github_id_rsa
    User brightl3016@outlook.com
# ssh.github.com：注释，说明此部分配置是针对通过 ssh.github.com 连接 GitHub 的
Host github.com：定义一个主机别名，仍然是针对 github.com 的，但通过不同的方式连接
HostName ssh.github.com：实际主机名，这里使用 ssh.github.com 来连接 GitHub
Port 443：指定连接端口为 443（通常是 HTTPS 端口），用来规避防火墙限制
PreferredAuthentications publickey：同样优先使用公钥认证
IdentityFile ~/.ssh/github_id_rsa：指定用于连接的私钥文件路径
User brightl3016@outlook.com：指定用于连接的用户名

# 总结
第一部分使用标准的端口（22）直接连接到 github.com。
第二部分则通过 HTTPS 端口（443）连接到 ssh.github.com，这对某些防火墙设置严格的网络环境很有用

## 二、Git 代码提交、合并、拉取    
在 Git 中，你可以通过以下步骤进行代码的提交、合并和拉取      

### 1、拉取代码     
- 在拉取最新代码之前，需要先将你的本地仓库链接到远程仓库。这通常在你第一次克隆仓库时完成。如果你已经链接到远程仓库，你可以用以下命令拉取最新的代码                
  ```bash
  git pull origin <branch_name>
  ```

### 2、创建并切换到新的开发分支    
- 为了避免直接在主分支上开发，你需要创建一个新的分支并切换到这个分支          
  ```bash
  git checkout -b <new_branch_name>
  ```

### 3、添加和提交代码    
- 在你完成代码修改后，你需要将这些修改添加到暂存区然后提交      
  ```bash
  git add .
  ```
- 上述命令会将所有修改过的文件添加到暂存区。如果你只想添加特定的文件，你可以替换 `.` 为文件名      
- 接下来，你可以提交你的更改。      
  ```bash
  git commit -m "<commit_message>"
  ```

### 4、推送代码     
- 在你完成修改并进行了提交后，你可以将你的代码推送到远程仓库       
  ```bash
  git push origin <new_branch_name>
  ```

### 5、合并代码   
- 在你完成所有开发并确保所有测试都通过后，你可以将你的分支合并到主分支。首先，你需要切换回主分支         
  ```bash
  git checkout <main_branch_name>
  ```
- 然后，你可以合并你的开发分支。    
  ```bash
  git merge <new_branch_name>
  ```
- 注意：在做合并之前，最好拉取一下主分支的最新代码并解决可能出现的冲突      