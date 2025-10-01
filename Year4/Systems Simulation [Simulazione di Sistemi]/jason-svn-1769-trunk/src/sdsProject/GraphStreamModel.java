/*
 * Copyright (C) 2014  Giulio Biagini, Giacomo Bergami, Gianluca Iselli
 * 
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 * 
 * Giulio Biagini  - giulio.biagini@studio.unibo.it
 * Giacomo Bergami - giacomo.bergami@studio.unibo.it
 * Gianluca Iselli - gianluca.iselli@studio.unibo.it
 */



package sdsProject;



import jason.asSemantics.Message;

import java.io.IOException;
import java.io.StringWriter;
import java.lang.reflect.Constructor;
import java.lang.reflect.InvocationTargetException;
import java.util.Arrays;

import org.graphstream.graph.Edge;
import org.graphstream.graph.ElementNotFoundException;
import org.graphstream.graph.Graph;
import org.graphstream.graph.Node;
import org.graphstream.graph.implementations.MultiGraph;
import org.graphstream.stream.file.FileSink;
import org.graphstream.ui.swingViewer.Viewer;



public class GraphStreamModel
{
	/*
	 * ************
	 * * COSTANTI *
	 * ************
	 */
	
	
	
	/**
	 * costante per i nomi dei formati in cui è possibile ottenere la rappresentazione del grafo
	 */
	public static final String[] FORMAT_NAMES = new String[]{"DGS", "DOT", "DynamicGML", "GEXF", "GML", "GraphML", "SVG", "SVG2", "SWF", "TikZ"};
	
	/**
	 * costante per le estensioni dei formati in cui è possibile ottenere la rappresentazione del grafo
	 */
	public static final String[] FORMAT_EXTENSIONS = new String[]{"dgs", "dot", "dgml", "gexf", "gml", "xml", "svg", "svg2", "swf", "tex"};
	
	/**
	 * costante per i tipi di messaggi che gli agenti possono scambiarsi
	 */
	public static final String[] MESSAGE_TYPES;
	
	
	
	/**
	 * costante per il CSS dello stile di visualizzazione del grafo
	 */
	private static final String CSS = "graph { fill-mode: dyn-plain; }" +
									  "node { size: 10px; text-background-mode: rounded-box; text-alignment: at-right; shape: box; fill-mode: dyn-plain; fill-color: green, red; }" +
									  "edge { size: 1px; fill-color: #444; text-alignment:above; }" +
									  "edge.sent { fill-color: blue; }" +
									  "node.sent { fill-color:blue,blue; }";
	
	
	
	/*
	 * *************
	 * * VARIABILI *
	 * *************
	 */
	
	
	
	/**
	 * il grafo
	 */
	private static Graph graph = new MultiGraph("graph");
	
	/**
	 * il contatore per l'id dei nodi generati
	 */
	private static int nodeId = 0;
	
	/**
	 * il contatore per l'id degli archi generati
	 */
	private static int edgeId = 0;
	
	/**
	 * il filtro sul tipo dei messaggi
	 */
	private static String filter = "";
	
	/**
	 * il mutex per la sincronizzazione dei metodi
	 */
	private static Object mutex = new Object();
	
	
	
	/*
	 * ***************
	 * * COSTRUTTORE *
	 * ***************
	 */
	
	
	
	/**
	 * setto le proprietà del grafo e visualizzo il fame
	 */
	static
	{
		// aggiungo alla lista dei tipi di messaggi anche la stringa vuota
		MESSAGE_TYPES = Arrays.copyOf(Message.knownPerformatives, Message.knownPerformatives.length + 1);
		MESSAGE_TYPES[Message.knownPerformatives.length] = "";
		
		// inizializzo l'interfaccia grafica
		System.setProperty("gs.ui.renderer", "org.graphstream.ui.j2dviewer.J2DGraphRenderer");
		
		// inizializzo il grafo
		graph.addAttribute("ui.stylesheet", CSS);
		graph.addAttribute("ui.quality");
		graph.addAttribute("ui.antialias");
		
		// se è stata specificata la proprietà e non le è stato assegnato valore "false" lancio la vista sul grafo
		String property = System.getProperty("graphStreamView.visible");
		if (property != null && !property.equalsIgnoreCase("false")) {
			Viewer viewer = new Viewer(graph, Viewer.ThreadingModel.GRAPH_IN_ANOTHER_THREAD);
			viewer.enableAutoLayout();
			new GraphStreamView(viewer.addDefaultView(false));
		}
	}
	
	
	
	/*
	 * ************
	 * * FUNZIONI *
	 * ************
	 */
	
	
	
	/**
	 * funzione che si occupa di tornare il mutex per la sincronizzazione dei metodi
	 * 
	 * @return il mutex per la sincronizzazione dei metodi
	 */
	public static Object getMutex()
	{
		return mutex;
	}
	
	/**
	 * funzione che si occupa di aggiungere un agente al grafo
	 * 
	 * @return l'id dell'agente
	 */
	public static String addAgent()
	{
		synchronized (mutex) {
			String newId = Integer.toString(nodeId++);
			graph.addNode(newId);
			return newId;
		}
	}
	
	/**
	 * funzione che si occupa di settare un nome all'agente.<br>
	 * se il nome in ingresso è null o vuoto, all'agente non verrà settato alcun nome
	 * 
	 * @param id - l'id dell'agente
	 * @param name - il nome dell'agente
	 * @return true se è stato settato il nuovo nome all'agente, false se l'id in ingresso non è valido
	 */
	public static boolean setAgentName(String id, String name)
	{
		synchronized (mutex) {
			try {
				if (name != null && !name.isEmpty())
					graph.getNode(id).setAttribute("ui.label", name);
			} catch (ElementNotFoundException ex) {
				return false;
			}
			
			return true;
		}
	}
	
	/**
	 * funzione che si occupa di rimuovere un nodo dal grafo
	 * 
	 * @param id - l'id del nodo
	 * @return true se è l'agente è stato rimosso, false se l'id in ingresso non è valido
	 */
	public static boolean removeAgent(String id)
	{
		synchronized (mutex) {
			try {
				graph.removeNode(id);
			} catch (ElementNotFoundException ex) {
				return false;
			}
			
			return true;
		}
	}
	
	/**
	 * funzione che si occupa di visualizzare come label su un arco l'invio di un messaggio tra due agenti.<br>
	 * se l'arco tra src e dest non eiste lo crea.<br>
	 * se il filtro sui messaggi è null, vuoto o se il messaggio è del tipo specificato nel filtro, viene visualizzato.<br>
	 * nel caso esistano più agenti con lo stesso nome, verrà considerato il primo agente trovato.<br>
	 * l'esecuzione del thread si blocca per un secondo nella visualizzazione del messaggio
	 * 
	 * @param srcAgentName - il nome dell'agente che invia il messaggio
	 * @param destAgentName - il nome dell'agente che riceve il messaggio
	 * @param type - il tipo di messaggio
	 * @param content - il contenuto del messaggio
	 * @return true se il messaggio è stato visualizzato, false se l'id o gli id in ingresso non sono validi
	 */
	public static boolean sendMessage(String srcAgentName, String destAgentName, String type, String content)
	{
		synchronized (mutex) {
			// ottengo gli id dei nodi src e dest dal nome
			String srcId = getNodeIdFromName(srcAgentName);
			String destId = getNodeIdFromName(destAgentName);
			Edge edge;
			
			// se uno o entrambi i nodi non esistono
			if (srcId == null || destId == null)
				return false;
			
			// se il tipo di messaggio rispetta il filtro
			if (filter(type)) {
				// ottengo l'arco tra src e dest, se non esiste lo creo
				if ((edge = graph.getNode(srcId).getEdgeBetween(destId)) == null)
					edge = graph.addEdge(Integer.toString(edgeId++), srcId, destId);
				
				// setto come label il contenuto del messaggio
				edge.setAttribute("ui.label", content + "");
				
				// aspetto 1 secondo
				String property = System.getProperty("graphStreamView.visible");
				if (property != null && !property.equalsIgnoreCase("false"))
					try { Thread.sleep(1000); } catch (InterruptedException e) { }
			}
			
			return true;
		}
	}
	
	/**
	 * funzione che si occupa di cancellare la visualizzazione del messaggio come label su un arco.<br>
	 * nel caso esistano più agenti con lo stesso nome, verrà considerato il primo agente trovato
	 * 
	 * @param srcAgentName - il nome dell'agente che invia il messaggio
	 * @param destAgentName - il nome dell'agente che riceve il messaggio
	 * @return true se il messaggio è stato cancellato, false se l'id o gli id in ingresso non sono validi
	 */
	public static boolean receiveMessage(String srcAgentName, String destAgentName)
	{
		synchronized (mutex) {
			// ottengo gli id dei nodi src e dest dal nome
			String srcId = getNodeIdFromName(srcAgentName);
			String destId = getNodeIdFromName(destAgentName);
			Edge edge;
			
			// se uno o entrambi i nodi non esistono
			if (srcId == null || destId == null)
				return false;
			
			// ottengo l'arco tra src e dest e cancello il messaggio dalla lebel
			if ((edge = graph.getNode(srcId).getEdgeBetween(destId)) != null)
				edge.removeAttribute("ui.label");
			
			return true;
		}
	}
	
	/**
	 * funzione che si occupa di settare un filtro sul tipo di messaggi.<br>
	 * nel caso in cui il filtro in input sia null o vuoto, non sarà applicato nessun filtro
	 * 
	 * @param filter - il filtro
	 */
	public static void setFilter(String filter)
	{
		synchronized (mutex) {
			GraphStreamModel.filter = filter;
		}
	}
	
	/**
	 * funzione che si occupa di ottenere il grafo nel formato specificato in input.<br>
	 * i formati possibili sono contenuti nell'array FORMAT_NAMES
	 * 
	 * @param formatName - il formato della stringa tra quelli nell'array FORMAT_NAMES
	 * @return il grafo nel formato specificato in input
	 * @throws Exception
	 */
	public static String getGraphContent(String formatName) throws ClassNotFoundException, NoSuchMethodException, SecurityException, InstantiationException, IllegalAccessException, IllegalArgumentException, InvocationTargetException, IOException
	{
		synchronized (mutex) {
			Class<?> clas;
			Constructor<?> constructor;
			
			StringWriter writer = new StringWriter();
			
			/* http://graphstream-project.org/api/gs-core/org/graphstream/stream/file/FileSink.html */
			clas = Class.forName("org.graphstream.stream.file.FileSink" + formatName);
			constructor = clas.getConstructor();
			FileSink object = (FileSink)constructor.newInstance();
			object.writeAll(graph, writer);
			
			return writer.toString();
		}
	}
	
	
	
	/*
	 * *********
	 * * UTILS *
	 * *********
	 */
	
	
	
	/**
	 * funzione che si occupa di verificare se una stringa supera il test del filtro.<br>
	 * una stringa passa tale test se il filtro è null, vuoto o se essa contiene la stringa del filtro
	 * 
	 * @param string - la stringa da filtrare
	 * @return true se la stringa ha passato il test del filtro, false altrimenti
	 */
	private static boolean filter(String string)
	{
		return (filter == null || filter.isEmpty() || string.contains(filter));
	}
	
	/**
	 * funzione che si occupa di tornare l'id del nodo con nome in input.<br>
	 * nel caso esistano più nodi con lo stesso nome, viene tornato l'id del primo nodo, se il nome non appartiene ad alcun
	 * agente viene restituito null
	 * 
	 * @param name - il nome dell'agente
	 * @return l'id del primo nodo con nome in input, null se non esistono nodi con quel nome
	 */
	private static String getNodeIdFromName(String name)
	{
		if (name != null) {
			// cerco il nodo con nome specificato
			for (int i = 0; i < graph.getNodeCount(); i++) {
				Node node = graph.getNode(i);
				if (node.getAttribute("ui.label").equals(name))
					return node.getId();
			}
		}
		
		// se nessun nodo ha il nome specificato
		return null;
	}
}