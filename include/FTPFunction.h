#ifndef FTP_FUNCTION_H
#define FTP_FUNCTION_H

#include <WinSock2.h>
#include <fstream>
#include <regex>
#include <string>

namespace ftpclient
{

    enum class ConnectToServerRes
    {
        //成功连接到服务器
        SUCCEEDED,
        WSAStartup_FAILED,
        getaddrinfo_FAILED,
        socket_FAILED,
        //无法连接服务器
        UNABLE_TO_CONNECT_TO_SERVER
    };

    /**
     * @brief 创建socket并连接到服务器
     * @author zhb
     * @param sock 出口参数，创建的socket
     * @param hostname 主机名
     * @param port 端口号，形如"21"或"ftp"均可
     * @param sendTimeout 阻塞式send()超时时间(ms)，负数表示不设置
     * @param recvTimeout 阻塞式recv()超时时间(ms)，负数表示不设置
     * @return 结果状态码
     */
    ConnectToServerRes connectToServer(SOCKET &sock,
                                       const std::string &hostname,
                                       const std::string &port, int sendTimeout,
                                       int recvTimeout);

    enum class CmdToServerRet
    {
        SUCCEEDED,
        FAILED_WITH_MSG,
        SEND_FAILED,
        RECV_FAILED
    };

    /**
     * @brief 向服务器发送命令并收取回复
     * @param controlSock 控制连接
     * @param sendCmd 命令，必须以"\r\n"结尾
     * @param matchRegex 匹配服务器消息的正则
     * @param recvMsg 出口参数，收到的消息
     * @return 结果状态码
     */
    CmdToServerRet cmdToServer(SOCKET controlSock, const std::string &sendCmd,
                               const std::regex &matchRegex,
                               std::string &recvMsg);

    //    enum class LoginToServerRes
    //    {
    //        //登录成功
    //        SUCCEEDED,
    //        //登录失败
    //        FAILED_WITH_MSG,
    //        // send()失败
    //        SEND_FAILED,
    //        // recv()失败
    //        RECV_FAILED
    //    };

    /**
     * @brief 连接到服务器并登录，该函数为阻塞式
     * @author zhb
     * @param controlSock 控制连接
     * @param username 用户名
     * @param password 密码
     * @param errorMsg 出口参数，来自服务器的错误信息
     * @return 结果状态码
     */
    CmdToServerRet loginToServer(SOCKET controlSock,
                                 const std::string &username,
                                 const std::string &password,
                                 std::string &errorMsg);

    /**
     * @brief 让服务器进入PASV模式
     * @author zhb
     * @param controlSock 控制连接
     * @param hostname 出口参数，数据连接的主机名（IPv4地址）
     * @param port 出口参数，数据连接的端口号
     * @param errorMsg 出口参数，来自服务器的错误信息
     * @return 结果状态码
     */
    CmdToServerRet putServerIntoPasvMode(SOCKET controlSock, int &port,
                                         std::string &hostname,
                                         std::string &errorMsg);

    /**
     * @brief 让服务器进入EPSV模式
     * @author zhb
     * @param controlSock 控制连接
     * @param port 出口参数，数据连接的端口号
     * @param errorMsg 出口参数，来自服务器的错误信息
     * @return 结果状态码
     */
    CmdToServerRet putServerIntoEpsvMode(SOCKET controlSock, int &port,
                                         std::string &errorMsg);

    enum class RequestToUpRes
    {
        SUCCEEDED,
        FAILED_WITH_MSG,
        SEND_FAILED,
        RECV_FAILED
    };

    /**
     * @brief 向服务器发送 STOR 命令，请求上传文件
     * @author zhb
     * @param controlSock 控制连接
     * @param remoteFilename 服务器文件名
     * @param errorMsg 出口参数，来自服务器的错误消息
     * @return 结果状态码
     */
    RequestToUpRes requestToUploadToServer(SOCKET controlSock,
                                           const std::string &remoteFilename,
                                           std::string &errorMsg);

    enum class UploadFileDataRes
    {
        SUCCEEDED,
        SEND_FAILED,
        READ_FILE_ERROR
    };

    /**
     * @brief 将文件数据上传到服务器
     * @author zhb
     * @param dataSock 数据连接
     * @param ifs 文件输入流
     * @return 结果状态码
     *
     * 目前还没法暂停或停止上传
     */
    UploadFileDataRes uploadFileDataToServer(SOCKET dataSock,
                                             std::ifstream &ifs);

    /**
     * @brief 获取服务器上某个文件的大小
     * @author zhb
     * @param controlSock 控制连接
     * @param filename 服务器上的文件名
     * @param filesize 出口参数，文件大小
     * @param errorMsg 出口参数，来自服务器的错误消息
     * @return 结果状态码
     */
    CmdToServerRet getFilesizeOnServer(SOCKET controlSock,
                                       const std::string &filename,
                                       int &filesize, std::string &errorMsg);

} // namespace ftpclient

#endif // FTP_FUNCTION_H
