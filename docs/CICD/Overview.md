# CI/CD

**术语介绍**

| 术语     | 含义                                                         |
| -------- | ------------------------------------------------------------ |
| commit   | git提交，默认提交时会出发pipeline                            |
| Job      | Runner execute 的内容                                        |
| Pipeline | 不同 stage 下的不同 job 组成的集合，即一次流水线             |
| Runner   | 单独执行每个 job 的 agent 或服务器，一个 job 最终会分配到某个 Runner 执行 |
| Stage    | 一个阶段 stage，里面可能会包含多个 job，同一个 stage 里的 job 会并行执行 |

## Install and Configure GitLab

[How To Install and Configure GitLab on Ubuntu 20.04](https://www.digitalocean.com/community/tutorials/how-to-install-and-configure-gitlab-on-ubuntu-20-04)

## Install Jfrog Artifactory

[Install JFrog Artifactory on Ubuntu 22.04|20.04|18.04](https://computingforgeeks.com/how-to-install-jfrog-artifactory-on-ubuntu/)

## 参考链接

[1] [DevOps之Gitlab-CICD实践篇](https://zhuanlan.zhihu.com/p/105157319)
