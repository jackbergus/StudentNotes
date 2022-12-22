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



package jason.architecture;



import jason.asSemantics.Message;
import sdsProject.GraphStreamModel;



public class GraphStreamAgArch extends AgArch
{
	/*
	 * ************
	 * * COSTANTI *
	 * ************
	 */
	
	
	
	/**
	 * l'id con il quale l'agente si identifica nel grafo
	 */
	protected final String ID;
	
	
	
	/*
	 * ***************
	 * * COSTRUTTORE *
	 * ***************
	 */
	
	
	
	/**
	 * crea un AgArch dove l'agente compare all'interno del grafo
	 */
	public GraphStreamAgArch()
	{
		super();
		ID = GraphStreamModel.addAgent();
	}
	
	
	
	/*
	 * ************
	 * * FUNZIONI *
	 * ************
	 */
	
	
	
	/**
	 * funzione che si occupa di settare un nome all'agente (per la visualizzazione su grafo).<br>
	 * se il nome in ingresso è null o vuoto, all'agente non verrà settato alcun nome
	 * 
	 * @param name - il nome dell'agente
	 */
	public void setAgName(String name)
	{
		GraphStreamModel.setAgentName(ID, name);
	}
	
	/**
	 * funzione che si occupa di visualizzare su grafo l'invio di un messaggio fra due agenti
	 * 
	 * @param message - il messaggio
	 */
	@Override public void sendMsg(Message message) throws Exception
	{
		GraphStreamModel.sendMessage(message.getSender(), message.getReceiver(), message.getIlForce(), message.getPropCont() + "");
	}
	
	/**
	 * funzione che si occupa di cancellare su grafo il messaggio inviato tra due agenti
	 * 
	 * @param message - il messaggio
	 */
	public void receiveMsg(Message message)
	{
		GraphStreamModel.receiveMessage(message.getSender(), message.getReceiver());
	}
	
	/**
	 * funzione che si occupa di cancellare su grafo la visualizzazione di un agente e dei suoi archi (messaggi)
	 */
	@Override public void stop()
	{
		GraphStreamModel.removeAgent(ID);
    }
}