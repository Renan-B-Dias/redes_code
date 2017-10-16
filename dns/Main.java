import java.net.UnknownHostException;
import java.net.Inet6Address;
import java.net.InetAddress;

public class Main {
	public static void main(String[] args) {

		if(args.length > 0) {
			String hostAddress = args[0];

			if(hostAddress.substring(hostAddress.length()-1).equals("."))
				hostAddress = hostAddress.substring(0, hostAddress.length()-1);

			try {
				InetAddress[] addressReturn = Inet6Address.getAllByName(hostAddress);
				for(InetAddress address: addressReturn) {
					System.out.println(address);
				}

			} catch(UnknownHostException exception) {
				System.out.println("UnknownHostException");
				System.out.println(exception);
			}

		} else {
			System.out.println("Missing params");
			System.out.println("Expected: <hostAddres> <ipaddress(Optional)>");
		}
	}
}