# API Authentication and Authorization

**认证(Authentication)**：指证明正确身份

**授权(Authorization)**：指允许某种动作

认证先于授权，也就是说，用户必须先处于合法状态，然后才能根据其授权级别被授予对资源的访问权限。

## 4种最常用的身份认证方法

### Basic Authentication

HTTP基本身份验证由于其固有的安全漏洞而很少被推荐。

这是最直接的方法，也是最简单的方法。使用此方法，**发送方将username：password放入请求标头中**。用户名和密码使用Base64编码，这是一种编码技术，可将用户名和密码转换为一组64个字符，以确保安全传输。

这种方法不需要cookie、会话ID、登录页面和其他类似的专业解决方案，并且因为它使用HTTP头本身，所以不需要握手或其他复杂的响应系统。这种方法是无状态的，因此客户端必须为每个请求提供凭据。它适用于 API 调用以及不需要持久会话的简单身份验证工作流。

下面是请求头中的Basic Auth示例：
`Authorization: Basic bG9sOnNlY3VyZQ==`

### Digest Authentication

HTTP Digest Auth（或 Digest Access Auth）是 HTTP 基本验证的一种更安全的形式。主要区别在于 HTTP 摘要验证的密码是以 MD5 哈希形式代替纯文本形式发送的，因此它比基本身份验证更安全。

#### Bear Authentication

承载身份验证（也称为令牌身份验证）是一种HTTP身份验证方案，涉及称为承载令牌的安全令牌。

名称“承载者认证”可以理解为“给予该令牌的承载者访问权限”。允许访问某个资源或URL的承载令牌，很可能是一个加密字符串，通常由服务器响应登录请求而生成。

客户端在向受保护的资源发出请求时，必须在Authorization标头中发送此令牌：
`Authorization: Bearer <token>`

承载认证方案最初是作为RFC-6750中OAuth 2.0的一部分创建的，但有时也单独使用。

与基本身份验证类似，承载身份验证应仅通过HTTPS（SSL）使用。

### API Keys

在[REST API安全](https://www.restcase.com/)中- API密钥在行业中得到广泛使用，并成为某种标准，但是，这种方法不应被视为一种良好的安全措施。

API密钥的创建是为了解决HTTP基本身份验证和其他此类系统的早期身份验证问题。在该方法中，向每个第一次用户分配唯一的生成值，表示用户是已知的。当用户试图重新进入系统时，他们的唯一密钥（有时由他们的硬件组合和IP数据生成，其他时候由知道他们的服务器随机生成）用于证明他们与以前是同一个用户。

许多API密钥作为URL的一部分在查询字符串中发送，这使得不应该访问它的人更容易发现它。请不要在查询字符串参数中放入任何API密钥或敏感信息！更好的选择是将API密钥放在Authorization头中。事实上，这就是建议的标准：`Authorization: Apikey 1234567890abcdef`.

然而，在实践中，API密钥出现在各种地方：

- Authorization Header授权标头
- Basic Auth基本身份验证
- Body Data数据体
- Custom Header自定义标题
- Query String查询字符串

使用API密钥肯定有一些合理的理由。首先，API密钥很简单。使用单个标识符很简单，对于某些用例，这是最佳解决方案。例如，如果API在功能上受到特定限制，其中“读取”是唯一可能的命令，则API密钥可以是适当的解决方案。不需要编辑、修改或删除，安全性是一个较低的问题。

然而，问题是，任何向服务发出请求的人都会传输他们的密钥，理论上，这个密钥可以像任何网络传输一样容易地被拾取，如果整个网络中的任何一点都不安全，整个网络就会暴露。

### OAuth(2.0)

OAuth最常见的实现使用以下一种或两种令牌：

- access token：像API密钥一样发送，它允许应用程序访问用户的数据;访问令牌可设置有效期。
- refresh token：作为OAuth流的可选部分，刷新令牌在其已过期的情况下检索新的访问令牌。 OAuth2结合了身份验证和授权，允许更复杂的范围和有效性控制。

OAuth 2.0是识别个人用户帐户并授予适当权限的最佳选择。在该方法中，用户登录到系统中。然后，该系统将请求认证，通常以令牌的形式。然后，用户将此请求转发到身份验证服务器，该服务器将拒绝或允许此身份验证。从这里，令牌被提供给用户，然后提供给请求者。这样的令牌然后可以由请求者在任何时间独立于用户进行检查以进行验证，并且可以在严格限制的范围和有效期的情况下随时间使用。

从根本上说，这是一个比其他方法更安全和强大的系统，主要是因为它允许建立可以提供对API服务不同部分的访问的范围，并且因为令牌在一定时间后被撤销-使得攻击者更难重用。

### OpenID Connect

OpenID Connect是OAuth 2.0协议之上的一个简单的身份层，它允许计算客户端基于授权服务器执行的身份验证来验证最终用户的身份，并以可互操作和类似REST的方式获得有关最终用户的基本配置文件信息。

在技术术语中，OpenID Connect指定了RESTful HTTP API，使用JSON作为数据格式。

OpenID Connect允许一系列客户端（包括基于Web的客户端、移动的客户端和JavaScript客户端）请求和接收有关已验证会话和最终用户的信息。该规范套件是可扩展的，支持诸如身份数据加密、OpenID提供者发现和会话管理等可选功能。

OpenID Connect定义了一个登录流，使客户端应用程序能够验证用户，并获得有关该用户的信息（或“声明”），如用户名、电子邮件等。用户身份信息编码在安全的JSON Web令牌（JWT）中，称为ID令牌。

## 参考链接

[1] [4 Most Used REST API Authentication Methods](https://blog.restcase.com/4-most-used-rest-api-authentication-methods/)

[2] [API authentication and authorization best practices](https://www.synopsys.com/blogs/software-security/api-authentication-authorization-best-practices/)

[3] [六种 Web 验证方法大揭秘](https://www.infoq.cn/article/xeirmzbscwxjoyc3hflv)