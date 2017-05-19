void sector1(){
   //connects to server with the ip address 130.195.6.196
   connect_to_server("130.195.6.196", 1024);
   //sends a message to the connected server
   send_to_server("Please");
   //receives message from the connected server
   char message[24];
   receive_from_server(message); 
   //reply to server
  send_to_server(message);
  sleep1(0, 100000);

  return;
}
