import javax.naming.Context;
import javax.naming.NamingEnumeration;
import javax.naming.NamingException;
import javax.naming.directory.Attribute;
import javax.naming.directory.Attributes;
import javax.naming.directory.DirContext;
import javax.naming.directory.InitialDirContext;
import java.util.Hashtable;

/**
 * Created by renan on 21/10/17.
 */
public class DnsMain {

    private static final String MX = "MX";
    private static final String A = "A";
    private static final String AAAA = "AAAA";

    public static void main(String[] args) throws NamingException {

        if(args.length > 0) {
            String hostAddress = args[0];
            String hostName = hostAddress;

            // Removes '.'
            if (hostAddress.substring(hostAddress.length() - 1).equals("."))
                hostAddress = hostAddress.substring(0, hostAddress.length() - 1);

            hostAddress = hostAddress.substring(4, hostAddress.length()); //Removes www

            Hashtable<String, Object> enviroment = new Hashtable<>();
            enviroment.put(Context.INITIAL_CONTEXT_FACTORY, "com.sun.jndi.dns.DnsContextFactory");
            enviroment.put(Context.PROVIDER_URL, "dns:");

            String attributes[] = new String[] {AAAA, A, MX};

            DirContext dirContext = new InitialDirContext(enviroment);
            Attributes dnsAttributes = dirContext.getAttributes(hostAddress, attributes);
            NamingEnumeration<? extends Attribute> returnedAttributes = dnsAttributes.getAll();

            System.out.println("Host name " + hostName);

            if(returnedAttributes.hasMore())
                System.out.println("AAAA: " + returnedAttributes.next().get());
            if(returnedAttributes.hasMore())
                System.out.println("A: " + returnedAttributes.next().get());
            if(returnedAttributes.hasMore())
                System.out.println("MX: " + returnedAttributes.next().get());

        } else {
            System.out.println("Missing params");
            System.out.println("Expected: <hostAddres>");
        }

    }

}
