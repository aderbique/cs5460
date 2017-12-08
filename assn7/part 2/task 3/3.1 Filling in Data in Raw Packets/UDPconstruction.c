struct ipheader {
type field;
.....
}
struct udpheader {
type field;
......
}
// This buffer will be used to construct raw packet.
char buffer[1024];
// Typecasting the buffer to the IP header structure
struct ipheader *ip = (struct ipheader *) buffer;
// Typecasting the buffer to the UDP header structure
struct udpheader *udp = (struct udpheader *) (buffer
+ sizeof(struct ipheader));
// Assign value to the IP and UDP header fields.
ip->field = ...;
udp->field = ...;

