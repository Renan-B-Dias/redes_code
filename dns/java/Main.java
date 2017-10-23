import javax.naming.directory.Attributes;
import javax.naming.directory.InitialDirContext;
import javax.naming.NamingEnumeration;
import javax.naming.NamingException;
import javax.naming.Context;
import java.net.InetAddress;
import java.net.Inet6Address;
import java.net.UnknownHostException;
import java.util.Hashtable;


public class Main {

	private static final String MX = "MX";

	public static void main(String[] args) {

		if(args.length > 0) {
			String hostAddress = args[0];

			if(hostAddress.substring(hostAddress.length()-1).equals("."))
				hostAddress = hostAddress.substring(0, hostAddress.length()-1);

			try {

				Hashtable<String, Object> enviroment = new Hashtable<>();
	            enviroment.put(Context.INITIAL_CONTEXT_FACTORY, "com.sun.jndi.dns.DnsContextFactory");
	            enviroment.put(Context.PROVIDER_URL, "dns:");

				InetAddress[] addressReturn = Inet6Address.getAllByName(hostAddress);

				if(addressReturn[0].getHostAddress() != null) {
					String ipv4 = addressReturn[0].getHostAddress();
					System.out.println("A: " + ipv4);
				} else {
					System.out.println("A: <none>");
				}

				if(addressReturn[1].getHostAddress() != null) {
					String ipv6 = addressReturn[1].getHostAddress();
					System.out.println("AAAA: " + ipv6);
				} else {
					System.out.println("AAAA: <none>");
				}

				hostAddress = hostAddress.substring(4, hostAddress.length());

				InetAddress inetAddress = addressReturn[0];
				InitialDirContext initialDirContext = new InitialDirContext(enviroment);
				Attributes attributes = initialDirContext.getAttributes(hostAddress, new String[] {MX});
				
				NamingEnumeration attributeEnumeration = attributes.getAll();

				if(attributeEnumeration.hasMore())
					while(attributeEnumeration.hasMore()) {
						System.out.println("MX: " + attributeEnumeration.next());
					}
				else
					System.out.println("MX: <none>");

				attributeEnumeration.close();

			}
			catch(UnknownHostException exception) {
				System.out.println(exception);
			}

			catch(NamingException exception) {
				System.out.println(exception);
			}

		} else {
			System.out.println("Missing params");
			System.out.println("Expected: <hostAddres> <ipaddress(Optional)>");
		}
	}
}