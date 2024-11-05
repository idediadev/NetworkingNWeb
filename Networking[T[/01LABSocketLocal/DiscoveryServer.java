//il discovery server è il main thread
//è il DNS di questa esercitazione, un sistema di nomi che smista i server per trovare quello adatto alla rihiesta ricevuta
//gestire meglio i corner cases 

//importation
import java.io.BufferedReader;
import java.io.ByteArrayOutputStream;
import java.io.DataOutputStream;
import java.io.FileReader;
import java.net.DatagramSocket;
import java.net.DatagramPacket;
import java.net.InetAddress;
import java.net.MulticastSocket;
import java.lang.Thread;


public class DiscoveryServer 
{
    private static final int BUFFER_SIZE = 1024;
    private int discoveryPort;
    private String fileNames[];
    private Integer gateToken[]; 

    //Metodo per la generazione del Discovery serve assovia una porta au un client
    public DiscoveryServer(int discoveryPort, String[] fileNames, int[] gateToken)
     {
        this.discoveryPort = discoveryPort;
        this.fileNames = fileNames;          //array di Stringhe associato al nome di file passato 
        this.gateToken = gateToken;         //array di Interi associate alle porte della macchina           
    }

    //metodo che richiede la porta associata a txt richiesto
    public int getPortForFile(String fileName)
     {
        for (int i = 0; i < fileNames.length; i++) {        
            if (fileNames[i].equals(fileName)) {            
                return gateToken[i];                           
            }
        }
        return -1;  // Se non trova il file, restituisce -1
    }

    //esecuzione 
    public void start() {
        try {
            DatagramSocket socket = new DatagramSocket(discoveryPort);      //--> errore non gradisce Datagramsocket
            byte[] buffer = new byte[BUFFER_SIZE];

            System.out.println("DiscoveryServer in ascolto sulla porta " + discoveryPort);

            while (true) {
                // Client REQUEST
                DatagramPacket requestPacket = new DatagramPacket(buffer, buffer.length);
                socket.receive(requestPacket);
                String fileName = new String(requestPacket.getData(), 0, requestPacket.getLength());

                System.out.println("Richiesta ricevuta per il file: " + fileName);

                // Cerca la porta del RowSwapServer per il file richiesto
                int port = getPortForFile(fileName);
                String response;

                if (port != -1) {
                    response = "Porta del server: " + port;
                } else {
                    response = "File non disponibile";
                }

                // Invia la risposta al client
                byte[] responseBytes = response.getBytes();
                InetAddress clientAddress = requestPacket.getAddress();
                int clientPort = requestPacket.getPort();
                DatagramPacket responsePacket = new DatagramPacket(responseBytes, responseBytes.length, clientAddress, clientPort);
                socket.send(responsePacket);

                System.out.println("Risposta inviata: " + response);
            }
        } catch (Exception e) {
            e.printStackTrace();
        }
    }

    public static void main(String[] args) {
        if (args.length < 2 || args.length % 2 != 0) {
            System.out.println("Uso: DiscoveryServer <porta> <file1> <porta1> ... <fileN> <portaN>");
            return;
        }

        int discoveryPort = Integer.parseInt(args[0]);
        int numFiles = (args.length - 1) / 2;
        String[] fileNames = new String[numFiles];
        int[] ports = new int[numFiles];

        // Popola gli array di nomi di file e porte
        for (int i = 0; i < numFiles; i++) {
            fileNames[i] = args[1 + 2 * i];
            ports[i] = Integer.parseInt(args[2 + 2 * i]);
        }

        // Crea e avvia il DiscoveryServer
        DiscoveryServer discoveryServer = new DiscoveryServer(discoveryPort, fileNames, ports);
        discoveryServer.start();
    }
    
}
