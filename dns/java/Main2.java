// import java.net.UnknownHostException;
// import java.net.Inet6Address;
// import java.net.InetAddress;
// import java.net.InetSocketAddress;
// import java.net.Socket;
// import java.io.PrintWriter;
// import java.io.BufferedReader;
// import java.io.InputStreamReader;
import java.io.*;
import java.net.*;

public class Main2 {
	private static final int port = 53;

	public static void main(String[] args) {

		if(args.length > 0) {
			String hostAddress = args[0];

			if(hostAddress.substring(hostAddress.length()-1).equals("."))
				hostAddress = hostAddress.substring(0, hostAddress.length()-1);

			try {
				InetAddress[] addressReturn = Inet6Address.getAllByName(hostAddress);
				String ipv4 = addressReturn[0].getHostAddress();
				String ipv6 = addressReturn[1].getHostAddress();

				System.out.println(ipv4);
				System.out.println(ipv6);
			} catch(UnknownHostException exception) {
				System.out.println(exception);
			}

			try {
				
				Socket socket = new Socket();
				socket.connect(new InetSocketAddress(hostAddress , 80));
				
				DataInputStream dataInputStream = new DataInputStream(socket.getInputStream());
				DataOutputStream dataOutputStream = new DataOutputStream(socket.getOutputStream());

				String answer = dataInputStream.readUTF();

	   //          System.out.println("Answer:"+answer);

	   //          dataInputStream.close();
	   //          dataOutputStream.close();
	            socket.close();

			} catch(Exception e) {
				System.out.println(e);
			}

		} else {
			System.out.println("Missing params");
			System.out.println("Expected: <hostAddres> <ipaddress(Optional)>");
		}
	}
}