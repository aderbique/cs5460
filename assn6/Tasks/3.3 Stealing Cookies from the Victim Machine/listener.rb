require 'thread'
require 'socket'

server = TCPServer.open(5000)
begin
    Thread.start(server.accept) do |client|
        response = client.recv(100)
        puts response
        client.close
    end
end while(true)
