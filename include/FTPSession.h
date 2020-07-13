#ifndef FTPSESSION_H
#define FTPSESSION_H

#include "../include/FTPFunction.h"
#include <QObject>
#include <string>
#include <winsock2.h>

namespace ftpclient
{

    class FTPSession : public QObject
    {
        Q_OBJECT
    public:
        FTPSession();
        ~FTPSession();
        //禁止复制
        FTPSession(const FTPSession &) = delete;
        FTPSession &operator=(const FTPSession &) = delete;

        /**
         * @brief 创建socket、连接服务器
         * @author zhb
         * @param hostName 服务器主机名
         * @param port 端口号
         *
         * 异步函数，运行结束后会发射以下信号之一：
         * - connectionToServerSucceeded
         * - createSocketFailed
         * - unableToConnectToServer
         * - recvFailed
         */
        void connect(const std::string &hostName, int port = 21);

        /**
         * @brief 登录FTP服务器
         * @author zhb
         * @param userName 用户名
         * @param password 密码
         *
         * 异步函数，运行结束后会发射以下信号之一：
         * - loginSucceeded
         * - loginFailedWithMsg
         * - sendFailed
         * - recvFailed
         */
        void login(const std::string &userName, const std::string &password);

        /**
         * @brief 让服务器切换到被动模式，通过信号获取数据连接端口号
         * @author zhb
         *
         * 异步函数，运行结束后会发射以下信号之一：
         * - putPasvSucceeded
         * - putPasvFailedWithMsg
         * - sendFailed
         * - recvFailed
         */
        void getPasvDataPort();

        /**
         * @brief 关闭控制端口的连接
         * @author zhb
         *
         * 尚未完成
         */
        void close();

        std::string getHostName() { return hostName; }

    signals:

        /**
         * @brief 信号：连接服务器成功
         * @param welcomeMsg 来自服务器的欢迎信息
         */
        void connectionToServerSucceeded(std::string welcomeMsg);
        /**
         * @brief 信号：创建socket失败
         * @param result 失败原因
         */
        void createSocketFailed(ConnectToServerRes result);
        /**
         * @brief 信号：无法连接到服务器
         */
        void unableToConnectToServer();

        /**
         * @brief 信号：recv()失败
         */
        void recvFailed();
        /**
         * @brief 信号：send()失败
         */
        void sendFailed();

        /**
         * @brief 信号：登录成功
         */
        void loginSucceeded();
        /**
         * @brief 信号：登录失败
         * @param errorMsg 来自服务器的错误信息
         */
        void loginFailedWithMsg(std::string errorMsg);

        /**
         * @brief 信号：切换 PASV 模式成功
         * @param dataHostname 数据连接主机名
         * @param port 数据连接端口号
         */
        void putPasvSucceeded(std::string dataHostname, int port);
        /**
         * @brief 信号：切换 PASV 模式失败
         * @param errorMsg 来自服务器的错误消息
         */
        void putPasvFailedWithMsg(std::string errorMsg);

    private:
        SOCKET controlSock;
        bool isConnectToServer;
        std::string hostName;
    };

} // namespace ftpclient

#endif // FTPSESSION_H
