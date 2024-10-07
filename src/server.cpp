#include "server.hpp"

Server::Server(int port)
    : m_serverSocket(-1)
    , m_port(port)
    , m_clients()
    , m_password()
    , m_channels()
{
    m_serverAddress.sin_family = AF_INET;
    m_serverAddress.sin_addr.s_addr = INADDR_ANY;
    m_serverAddress.sin_port = htons(port);
    
    setupSocket(port);
}


Server::~Server()
{
	stop();
}

void Server::start()
{
	while (true)
	{
		acceptClient();
	}
}

void Server::stop()
{
	for (size_t i = 0; i < m_clients.size(); i++)
	{
		delete m_clients[i];
	}
	close(m_serverSocket);
    std::cout << "Server stopped --- Welcome to the real world " << std::endl;
}

void Server::setupSocket(int port)
{
    m_serverSocket = socket(AF_INET, SOCK_STREAM, 0);
    m_serverAddress.sin_family = AF_INET;
    m_serverAddress.sin_port = htons(port);
    m_serverAddress.sin_addr.s_addr = INADDR_ANY;

    bind(m_serverSocket, (struct sockaddr *)&m_serverAddress, sizeof(m_serverAddress));
    listen(m_serverSocket, 10);

    std::cout << "Server started on port " << port << std::endl;
}

void Server::acceptClient()
{
    int clientSocket = accept(m_serverSocket, NULL, NULL);
    if (clientSocket >= 0)
    {
        std::cout << "New client connected" << std::endl;
        ClientHandler* newClient = new ClientHandler(clientSocket, this);
        m_clients.push_back(newClient);
        newClient->handlerClient(); // Correction de la méthode appelée
    }
    else
    {
        std::cerr << "Error accepting client" << std::endl;
    }
}

void Server::setupPassword(const std::string& password)
{
    m_password = password;
}

Channel Server::getChannel(const std::string& name)
{
    for (size_t i = 0; i < m_channels.size(); i++)
    {
        if (m_channels[i].getName() == name)
        {
            return m_channels[i];
        }
    }
    throw std::runtime_error("Channel not found");
}

bool checkIfChannelExists(const std::vector<Channel> channels, const std::string& name)
{
    for (size_t i = 0; i < channels.size(); i++)
    {
        if (channels[i].getName() == name)
        {
            return true;
        }
    }
    return false;
}

void Server::joinChannel(const User& newUser, const std::string& name)
{
    if (checkIfChannelExists(m_channels, name))
    {
        std::cout << "Channel already exists add user in" << std::endl;
        getChannel(name).addUser(newUser);   
        return;
    }
    Channel newChannel(newUser, name);
    m_channels.push_back(newChannel);
}