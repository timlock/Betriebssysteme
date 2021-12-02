package de.hsos.bs;

import java.io.IOException;
import java.io.InputStream;
import java.io.OutputStream;
import java.io.PrintWriter;
import java.net.MalformedURLException;
import java.net.ServerSocket;
import java.net.Socket;
import java.net.URL;
import java.net.URLConnection;
import java.util.Random;

/**
 * Mehrstufiger Proxy-Server zur verzoegerten Verarbeitung von HTTP-GET
 * Anfragen.
 *
 * Ausfuehrbar mit 0 - 2 Parametern: 1. Parameter: Durchschnittliche
 * Verzoegerungszeit in ms, frei lassen = 0ms 2. Parameter: Seed des
 * Pseudo-Zufalls-Generators, frei lassen fuer "a value very likely to be
 * distinct from any other invocation"
 *
 * @author Julian Klose
 * @see java.util.Random#Random()
 */
public class ProxyServer extends Thread {

    /**
     * Hilfsklasse fuer normalverteilte Zufallszahlen.
     *
     * @author Julian Klose
     */
    protected static class RandomGausian {

        private Random rand;
        private int averageDelay;
        private int deviation;

        /**
         * Erstellt einen neuen Zufalls-Generator fuer normalverteilte
         * Verzoegerungszeiten.
         *
         * @param averageDelay Mittelwert der Verzoegerungszeiten
         * @param seed Seed des Zufalls-Generators, null fuer "a value very
         * likely to be distinct from any other invocation"
         * @see java.util.Random#Random()
         * @see java.util.Random#Random(long)
         * @see java.util.Random#nextGaussian()
         */
        public RandomGausian(int averageDelay, Integer seed) {
            if (seed != null) {
                rand = new Random(seed);
            } else {
                rand = new Random();
            }
            this.averageDelay = averageDelay;
            for (int i = 1; i < averageDelay; i *= 10) {
                deviation = i;
            }
        }

        /**
         * Gibt die naechste normalverteilte Zufalls-Verzoegerung aus.
         *
         * @return int mit der naechsten Verzoegerungszeit.
         * @see java.util.Random#nextGaussian()
         */
        public int nextInt() {
            if (averageDelay == 0) {
                return 0;
            }
            int delay = (int) (averageDelay + rand.nextGaussian() * deviation);
            return (delay < 0) ? 0 : delay;
        }
    }
    private static RandomGausian rand;

    /**
     * Startet den Proxy-Server
     *
     * @param args Start-Parameter
     * @see ProxyServer
     */
    public static void main(final String[] args) {
        int port = 8000;

        int averageDelay = 0;
        Integer seed = null;

        try {
            if (args.length > 0) {
                averageDelay = Integer.parseInt(args[0]);
            }
            if (args.length > 1) {
                seed = Integer.parseInt(args[1]);
            }

            rand = new RandomGausian(averageDelay, seed);

            System.out.println("Starting proxy on Port " + port);

            ServerSocket proxy = new ServerSocket(port);
            while (true) {
                Socket client = proxy.accept();
                new ProxyServer(client).start();
            }
        } catch (NumberFormatException e) {
            System.err.println("Als Parameter sind nur Integer-Zahlen erlaubt: ");
            System.err.println("Parameter 1: Mittlere Verzoegerung in Millisekunden");
            System.err
                    .println("Parameter 2: Seed des Zufallsgenerators, leer lassen fuer einmaligen Seed");
        } catch (IOException e1) {
            e1.printStackTrace();
        }
    }
    private Socket client;
    private InputStream streamFromClient;
    private OutputStream streamToClient;
    private InputStream streamFromServer = null;
    private String request;

    /**
     * Erstellt einen neuen Thread des mehrstufigen Proxy-Servers
     *
     * @param client Socket der Verbindung der eingehenden Anfrage
     */
    public ProxyServer(final Socket client) {
        this.client = client;
    }

    @Override
    public void run() {
        try {
            streamFromClient = client.getInputStream();
            streamToClient = client.getOutputStream();

            do {
                if (!readRequest()) {
                    break;
                }

                String page = getPage();
                if (page == null) {
                    sendError(400, "No Page found in Request Header.");
                    continue;
                }

                if (page.charAt(0) == '/') {
                    String host = getHost();
                    if (host == null) {
                        sendError(400, "No Host-Field found in Request Header.");
                        continue;
                    }
                    page = "http://" + host + page;
                }

                int delay = rand.nextInt();
                System.out.println("Proxy-Request " + page
                        + " with " + delay + " ms delay");
                if (delay > 0) {
                    Thread.sleep(delay);
                }

                try {
                    forwardResponse(page);
                } catch (MalformedURLException e) {
                    sendError(400, "Unknown protocol: " + page);
                } catch (IOException e1) {
                    sendError(400, "Server not found: " + page);
                }

            } while (false);
        } catch (IOException e) {
            e.printStackTrace();
        } catch (InterruptedException e1) {
            e1.printStackTrace();
        } finally {
            try {
                if (streamFromServer != null) {
                    streamFromServer.close();
                }
                client.close();
            } catch (IOException e) {
                e.printStackTrace();
            }
        }
    }

    /**
     * Liest einen HTTP/1.1-Header ein.
     *
     * @return true fuer erfolgreich eingelesenen Request, sonst false
     * @throws IOException
     * @see {@link java.io.InputStream#read()}
     */
    protected boolean readRequest() throws IOException {
        StringBuilder req = new StringBuilder();

        int c;
        do {
            c = streamFromClient.read();
            if (c < 0) {
                request = null;
                return false;
            }

            req.append((char) c);
        } while (req.length() < 4 || req.indexOf("\r\n\r\n", req.length() - 4) < 0);

        request = req.toString();
        return true;
    }

    /**
     * Liest den Host aus einem HTTP/1.1 Header.
     *
     * @return String mit Host
     */
    protected String getHost() {
        int start = request.indexOf("Host: ");
        if (start < 0) {
            return null;
        }
        int end = request.indexOf("\r\n", start);
        if (end < 0) {
            return null;
        }

        return request.substring(start + 6, end);
    }

    /**
     * Liest die angefragte Seite aus einem HTTP/1.1 Header.
     *
     * @return String mit angefragter Seite.
     */
    protected String getPage() {
        if (!request.startsWith("GET ")) {
            return null;
        }
        int end = request.indexOf(" HTTP/");
        if (end < 0) {
            end = request.indexOf('\r');
            if (end < 0) {
                end = request.indexOf('\n');
                if (end < 0) {
                    return null;
                }
            }
        }
        return request.substring(4, end);
    }

    /**
     * Sendet eine Fehlermeldung an den Client zurueck.
     *
     * @param statuscode HTTP-Fehler-Code
     * @param reason Beschreibung des Fehlers
     * @throws IOException
     * @see {@link java.io.PrintWriter#print(String)}
     */
    protected void sendError(final int statuscode, final String reason) throws IOException {
        System.err.println(statuscode + ": " + reason);
        PrintWriter out = new PrintWriter(streamToClient);
        out.print("HTTP/1.1 ");
        out.print(statuscode);
        out.print(" ");
        out.print(reason);
        out.print("\r\n\r\n");
        out.flush();
        out.close();
    }

    /**
     * Stellt eine Verbindung zum Host her und leitet die Antwort an den
     * Clienten weiter.
     *
     * @param url Angefragte Seite
     * @throws MalformedURLException Unbekanntes Protokoll
     * @throws IOException
     * @see {@link java.net.URL#openConnection()}
     */
    protected void forwardResponse(final String url) throws MalformedURLException, IOException {
        URLConnection con = new URL(url).openConnection();
        con.setReadTimeout(3000);
        InputStream streamFromServer = con.getInputStream();
        try {
            byte[] buffer = new byte[1024];
            int bytesread;
            while ((bytesread = streamFromServer.read(buffer)) >= 0) {
                streamToClient.write(buffer, 0, bytesread);
                streamToClient.flush();
            }
        } catch (IOException e) {
        }
        streamFromServer.close();
    }
}
