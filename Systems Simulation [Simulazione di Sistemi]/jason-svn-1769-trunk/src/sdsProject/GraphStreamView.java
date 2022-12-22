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



import java.awt.BorderLayout;
import java.awt.Toolkit;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.awt.event.MouseWheelEvent;
import java.awt.event.MouseWheelListener;
import java.io.BufferedWriter;
import java.io.FileWriter;

import javax.swing.JFileChooser;
import javax.swing.JFrame;
import javax.swing.JMenu;
import javax.swing.JMenuBar;
import javax.swing.JMenuItem;
import javax.swing.JOptionPane;
import javax.swing.JPanel;
import javax.swing.JScrollPane;
import javax.swing.JSlider;
import javax.swing.UIManager;
import javax.swing.UnsupportedLookAndFeelException;
import javax.swing.event.ChangeEvent;
import javax.swing.event.ChangeListener;
import javax.swing.filechooser.FileNameExtensionFilter;

import org.graphstream.ui.swingViewer.View;



@SuppressWarnings("serial")
public class GraphStreamView extends JFrame
{
	/*
	 * ************
	 * * COSTANTI *
	 * ************
	 */
	

	/**
	 * costante per il titolo del frame
	 */
	private static final String TITLE = "GRAPH STREAM VIEW";
	
	/**
	 * costante per il toolkit di default
	 */
	private static final Toolkit DEFAULTTOO_TOOLKIT = Toolkit.getDefaultToolkit();
	
	/**
	 * costante per la larghezza dello schermo
	 */
	private static final int SCREEN_WIDTH = DEFAULTTOO_TOOLKIT.getScreenSize().width;
	
	/**
	 * costante per l'altezza dello schermo
	 */
	private static final int SCREEN_HEIGHT = DEFAULTTOO_TOOLKIT.getScreenSize().height;
	
	/**
	 * costante per la larghezza del frame
	 */
	private static final int FRAME_WIDTH = 600;
	
	
	
	/*
	 * *************
	 * * VARIABILI *
	 * *************
	 */
	
	
	
	/**
	 * il menuItem con il quale settare il filtro sui messaggi
	 */
	private JMenuItem menuItemSetFilter;
	
	/**
	 * il menuItem con il quale salvare il grafo nel file
	 */
	private JMenuItem menuItemSaveGraph;
	
	/**
	 * il menuItem per chiudere la vista
	 */
	private JMenuItem menuItemCloseView;
	
	/**
	 * il menuItem per uscire dal programma
	 */
	private JMenuItem menuItemExit;
	
	/**
	 * il menu per le operazioni "generali"
	 */
	private JMenu menuFile;
	
	/**
	 * il menu per le operazioni sugli agenti
	 */
	private JMenu menuAgent;
	
	/**
	 * la menubar
	 */
	private JMenuBar menuBar;
	
	
	
	/**
	 * l'oggetto per scrollare la vista nella quale viene visualizzato il grafo
	 */
	private JScrollPane scrollGraphView;
	
	/**
	 * il pannello al centro del frame
	 */
	private JPanel centralPanel;
	
	
	
	/**
	 * lo slider per lo zoom del grafo
	 */
	private JSlider sliderZoom;
	
	/**
	 * il pannello in basso al frame
	 */
	private JPanel southPanel;
	
	
	
	/*
	 * ***************
	 * * COSTRUTTORE *
	 * ***************
	 */
	
	
	
	/**
	 * crea un frame per la visualizzazione di un grafo
	 * 
	 * @param graphView - 
	 */
	public GraphStreamView(final View graphView)
	{
		/*
		 * imposto il LookAndFeel di sistema
		 */
		
		try {
			UIManager.setLookAndFeel(UIManager.getSystemLookAndFeelClassName());
		} catch (ClassNotFoundException | InstantiationException | IllegalAccessException | UnsupportedLookAndFeelException ex) { }
		
		/*
		 * setto il menu file
		 */
		
		menuItemSaveGraph = new JMenuItem("Save graph...");
		menuItemSaveGraph.addActionListener(new ActionListener() {
			@Override public void actionPerformed(ActionEvent e) {
				actionSave();
			}
		});
		
		menuItemCloseView = new JMenuItem("Close view");
		menuItemCloseView.addActionListener(new ActionListener() {
			@Override public void actionPerformed(ActionEvent e) {
				actionCloseView();
			}
		});
		
		menuItemExit = new JMenuItem("Exit");
		menuItemExit.addActionListener(new ActionListener() {
			@Override public void actionPerformed(ActionEvent e) {
				actionExit();
			}
		});
		
		menuFile = new JMenu("File");
		menuFile.add(menuItemSaveGraph);
		menuFile.addSeparator();
		menuFile.add(menuItemCloseView);
		menuFile.add(menuItemExit);
		
		/*
		 * setto il menu graph
		 */
		
		menuItemSetFilter = new JMenuItem("Set message filter...");
		menuItemSetFilter.addActionListener(new ActionListener() {
			@Override public void actionPerformed(ActionEvent e) {
				actionSetFilter();
			}
		});
		
		menuAgent = new JMenu("Agent");
		menuAgent.add(menuItemSetFilter);
		
		/*
		 * setto la menubar
		 */
		
		menuBar = new JMenuBar();
		menuBar.add(menuFile);
		menuBar.add(menuAgent);
		
		/*
		 * nel pannello centrale visualizzo il grafo
		 */
		
		graphView.addMouseWheelListener(new MouseWheelListener() {
			@Override public void mouseWheelMoved(MouseWheelEvent e) {
				int newValue = sliderZoom.getValue() - (e.getWheelRotation() * e.getScrollAmount() * 20);
				if (newValue <= sliderZoom.getMaximum() && newValue >= sliderZoom.getMinimum())
					sliderZoom.setValue(newValue);// richiama il listener dello slider che chiama la "zoom" che modifica lo zoom del grafo
			}
		});
		
		scrollGraphView = new JScrollPane(graphView);
		scrollGraphView.setAutoscrolls(true);
		
		centralPanel = new JPanel(new BorderLayout());
		centralPanel.add(scrollGraphView, BorderLayout.CENTER);
		
		/*
		 * nel pannello in basso visualizzo lo slider
		 */
		
		sliderZoom = new JSlider(0, 10000);
		sliderZoom.setOrientation(JSlider.HORIZONTAL);
		sliderZoom.setValue(0);
        graphView.getCamera().setViewPercent((10000.0 - ((double)sliderZoom.getValue())) / 1000.0);
        sliderZoom.addChangeListener(new ChangeListener() {
            @Override public void stateChanged(ChangeEvent e) {
            	graphView.getCamera().setViewPercent((10000.0 - (double) sliderZoom.getValue()) / 1000.0);
            }
        });
        
        southPanel = new JPanel(new BorderLayout());
		southPanel.add(sliderZoom, BorderLayout.CENTER);
        
        /*
		 * imposto il JFrame
		 */
		
		setTitle(TITLE);
		setDefaultCloseOperation(JFrame.DISPOSE_ON_CLOSE);
		setBounds(SCREEN_WIDTH - FRAME_WIDTH, 0, FRAME_WIDTH, SCREEN_HEIGHT);
		setResizable(true);
		setLayout(new BorderLayout());
		setJMenuBar(menuBar);
		add(centralPanel, BorderLayout.CENTER);
		add(southPanel, BorderLayout.SOUTH);
		setVisible(true);
		
		/*
		 * visualizzo il frame per settare il filtro
		 */
		
		actionSetFilter();
	}
	
	
	
	/*
	 * **********
	 * * AZIONI *
	 * **********
	 */
	
	
	
	/**
	 * funzione che si occupa di settare il filtro sul tipo dei messaggi
	 */
	private void actionSetFilter()
	{
		Object mutex = GraphStreamModel.getMutex();
		
		synchronized (mutex) {
			String filter = JOptionPane.showInputDialog(this, "Choose how to filter messages by type", TITLE, JOptionPane.QUESTION_MESSAGE, null, GraphStreamModel.MESSAGE_TYPES, null) + "";
			GraphStreamModel.setFilter(filter);
		}
	}
	
	/**
	 * funzione che si occupa di salvare il grafo su file
	 */
	private void actionSave()
	{
		// creo il fileChooser
		JFileChooser fileChooser = new JFileChooser();
		fileChooser.setFileSelectionMode(JFileChooser.FILES_ONLY);
		fileChooser.removeChoosableFileFilter(fileChooser.getFileFilter());
		
		// aggiungo i filtri
		for (int i = 0; i < GraphStreamModel.FORMAT_NAMES.length; i++)
			fileChooser.addChoosableFileFilter(new FileNameExtensionFilter(GraphStreamModel.FORMAT_NAMES[i], GraphStreamModel.FORMAT_EXTENSIONS[i]));
		
		// se l'utente approva un path per il salvataggio
		if (fileChooser.showOpenDialog(new JFrame()) == JFileChooser.APPROVE_OPTION) {
			FileNameExtensionFilter choosenFilter = (FileNameExtensionFilter) fileChooser.getFileFilter();
			String formatName = choosenFilter.getDescription();
			String formatExtension = choosenFilter.getExtensions()[0];
			String absolutePath = fileChooser.getSelectedFile().getAbsolutePath();
			try {
				BufferedWriter bufferedWriter = new BufferedWriter(new FileWriter(absolutePath + "." + formatExtension));
				bufferedWriter.write(GraphStreamModel.getGraphContent(formatName));
				bufferedWriter.close();
				JOptionPane.showMessageDialog(this, "The file has been saved", TITLE, JOptionPane.INFORMATION_MESSAGE);
			} catch (Exception ex) {
				JOptionPane.showMessageDialog(this, ex.getMessage(), TITLE, JOptionPane.ERROR_MESSAGE);
			}
		}
	}
	
	/**
	 * funzione che si occupa di chiudere la vista
	 */
	private void actionCloseView()
	{
		dispose();
	}
	
	/**
	 * funzione che si occupa di terminare il programma
	 */
	private void actionExit()
	{
		if (JOptionPane.showConfirmDialog(this, "Terminare il programma?", TITLE, JOptionPane.YES_NO_CANCEL_OPTION) == JOptionPane.YES_OPTION)
			System.exit(0);
	}
}