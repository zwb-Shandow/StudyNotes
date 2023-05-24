# GitLab Runner

GitLab Runner 是一个应用程序，它与 GitLab CI/CD 一起运行管道中的作业。GitLab Runner 还可以在 Docker 容器中运行，也可以部署到Kubernetes 集群中。

GitLab Runner有以下类型的runner，根据您想要访问的用户提供：

- Shared runners: 所有组和项目都可以使用
- Group runners: 该组中的所有项目和子组
- Project runners: 特定项目可用

## 1. 注册 Runner

安装应用程序后，您可以注册单个 Runner。Runners 是运行来自GitLab的 CI/CD jobs 的代理。

运行程序通常在安装 GitLab Runner 的同一台计算机上处理作业。但是，您也可以在 Docker、Kubernetes 集群或云中的自动缩放实例中拥有一个runner 进程作业。

### 1.1 执行器

注册 runner 时，必须选择执行器。

执行器决定了每个 job 运行时的环境。

- 如果希望 CI/CD 作业运行 PowerShell 命令，可以在 Windows 服务器上安装 GitLab Runner，然后注册使用 shell 执行器的 runner。

- 如果希望 CI/CD 作业在自定义 Docker 容器中运行命令，可以在Linux 服务器上安装 GitLab Runner，并注册使用 Docker 执行器的 runner。

这些只是几种可能的配置。您可以在虚拟机上安装 GitLab Runner，并让它使用另一个虚拟机作为执行器。

当您在 Docker 容器中安装 GitLab Runner 并选择 Docker 执行器来运行作业时，有时称为“Docker in Docker”配置。

### 1.2 runner 权限

基于访问权限，有三种类型的 runner:

- Shared runners
- Group runners
- Specific runners

注册 runner 时，需要为 GitLab 实例、组或项目指定一个 token。这就是 runner 知道哪些项目可以使用它。

### 1.3 Tags

注册 runner 时，可以向其添加标签。

当 CI/CD 作业运行时，它通过查看分配的标记来知道要使用哪个 runner。

例如，如果一个 runner 有 ruby 标记，您可以将此代码添加到项目的 `.gitlab-ci.yml` 文件：

```yaml
job:
  tags:
    - ruby
```

当作业运行时，它使用带有 ruby 标记的 runner。

## 2. 配置 Runner

通过编辑 `config.toml` 文件配置 runner。该文件是 runner 安装过程中安装的。

在该文件中，可以编辑特定 runner 或所有 runner 的配置。您可以指定日志记录和缓存等设置。您可以设置并发、内存、CPU 限制等





https://docs.gitlab.cn/jh/ci/runners/runners_scope.html