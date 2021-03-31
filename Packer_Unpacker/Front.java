import java.lang.*;
import java.awt.*;
import javax.swing.*;
import java.awt.event.*;
import java.io.*;
import java.util.*;
import java.io.File;
import java.io.FileOutputStream;
import java.io.FileInputStream;
import java.io.Serializable;
import javax.swing.plaf.basic.BasicBorders;
import java.text.SimpleDateFormat;
import java.util.Date;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;

class Front
{
	public static void main(String arg[])
	{
		Window obj = new Window();
	}
}

class Window
{
	public Window()
	{
		JFrame jobj = new JFrame("Marvellous Login Page");

		JButton jobj1 = new JButton("Pack");
		jobj1.setBounds(10,200,140,40);

		JButton jobj2 = new JButton("Unpack");
		jobj2.setBounds(210,200,140,40);

		JButton jobj3 = new JButton("Exit");
		jobj3.setBounds(410,200,140,40);


		JLabel lobj = new JLabel("Please Select Your Option Below");
		lobj.setBounds(200,50,200,100);

		jobj.add(lobj);
		jobj.add(jobj1);
		jobj.add(jobj2);
		jobj.add(jobj3);

		jobj.setSize(600,300);
		jobj.setLayout(null);
		jobj.setVisible(true);
		jobj.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
		
		jobj1.addActionListener(new ActionListener(){
			public void actionPerformed(ActionEvent eobj){
				jobj.setVisible(false);
				Window1 o = new Window1();
			}
		});

		jobj2.addActionListener(new ActionListener(){
			public void actionPerformed(ActionEvent eobj){
				jobj.setVisible(false);
				NewWindow o = new NewWindow();
			}
		});


		jobj3.addActionListener(new ActionListener(){
			public void actionPerformed(ActionEvent eobj){
				jobj.setVisible(false);
				Window2 o = new Window2();
			}
		});
	}
}
class Window1 //implements ActionListener
{
	public Window1()
	{
		JFrame f = new JFrame("Marvellous Login");
		
		JButton bobj = new JButton("Pack");
		bobj.setBounds(100,200,140,40);
		// --------------------
		JLabel lobj1 = new JLabel("Enter Folder name");
		lobj1.setBounds(20,10,200,100);

		JTextField tf1 = new JTextField();
		// (x cordinate, y cordinate, widtyh, height)
		tf1.setBounds(150,50,130,30);
		// ------------------------------------
		JLabel lobj2 = new JLabel("Enter File name");
		lobj2.setBounds(20,80,100,100);

		JTextField tf2 = new JTextField();
		// (x cordinate, y cordinate, widtyh, height)
		tf2.setBounds(150,120,130,30);

		f.add(lobj1);
		f.add(bobj);
		f.add(tf1);
		f.add(lobj2);
		f.add(tf2);

		f.setSize(300,300);
		f.setLayout(null);
		f.setVisible(true);
		f.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);

		bobj.addActionListener(new ActionListener(){
			public void actionPerformed(ActionEvent eobj){
				Packer pobj = new Packer(tf1.getText(), tf2.getText());
				f.setVisible(false);
				Window o = new Window();
			}
		});
	}
}

class NewWindow
{
	public NewWindow()
	{
		JFrame fobj = new JFrame("Unpack");

		JButton bobj2 = new JButton("Unpack");
		bobj2.setBounds(100,200,140,40);
		
		JLabel lobj3 = new JLabel("Enter File Name To Unpack");
		lobj3.setBounds(20,20,200,100);

		JTextField tf3 = new JTextField();
		tf3.setBounds(200,60,130,30);

		fobj.add(lobj3);
		fobj.add(bobj2);
		fobj.add(tf3);
		
		fobj.setSize(400,300);
		fobj.setLayout(null);
		fobj.setVisible(true);
		fobj.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);

		bobj2.addActionListener(new ActionListener(){
			public void actionPerformed(ActionEvent eobj){
				Unpacker uobj = new Unpacker(tf3.getText());
				fobj.setVisible(false);

				Window o = new Window();
			}
		});
	}
}

class Window2
{
	public Window2()
	{
		JFrame fobj2 = new JFrame("Thank You"); 

		JLabel lobj4 = new JLabel("Thank You For Using Marvellous Packer Unpacker");
		lobj4.setBounds(40,20,300,200);
		JLabel lobj = new JLabel("Check Your Folder or File For Changes");
		lobj.setBounds(70,50,300,200);

		fobj2.add(lobj4);
		fobj2.add(lobj);
		fobj2.setSize(400,300);
		fobj2.setLayout(null);
		fobj2.setVisible(true);
		fobj2.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
	}
}

class Packer
{
	// Object for file writing
	public FileOutputStream outstream = null;

	// parametrised constructor
	public Packer(String FolderName, String FileName)
	{
		try
		{
			// Create new file for packing
			File outfile = new File(FileName);
			outstream = new FileOutputStream(FileName);

			// Set the current working directory for folder traversal
			// System.setProperty("user.dir",FolderName);
			
			TravelDirectory(FolderName);
		}
		catch(Exception obj)
		{
			System.out.println(obj);
		}
	}

	public void TravelDirectory(String path)
	{
		File directoryPath = new File(path);
		int counter = 0;
		// Get all file names from directory
		File arr[] = directoryPath.listFiles();

		System.out.println("-------------------------------");
		for(File filename : arr)
		{
			//System.out.println(filename.getAbsolutePath());
			
			if(filename.getName().endsWith(".txt"))
			{
				counter++;
				System.out.println("Packed file : "+filename.getName());
				PackFile(filename.getAbsolutePath());	
			}		
		}
		System.out.println("-------------------------------");
		System.out.println("Succesfully packed files : "+counter);
		System.out.println("-------------------------------");
	}

	public void PackFile(String FilePath)
	{
//		System.out.println("File name received "+ FilePath);
		byte Header[] = new byte[100];
		byte Buffer[] = new byte[1024];
		int length = 0;

		FileInputStream istream = null;

		File fobj = new File(FilePath);

		String temp = FilePath+" "+fobj.length();
		
			// Create header of 100 bytes
		for(int i = temp.length(); i< 100; i++)
		{
			temp = temp + " ";
		}	

		Header = temp.getBytes();
		try
		{
			// open the file for reading
			istream = new FileInputStream(FilePath);

			outstream.write(Header,0,Header.length);
			
			while((length = istream.read(Buffer)) > 0)
			{
				outstream.write(Buffer,0,length);
			}

			istream.close();
		}
		catch(Exception obj)
		{}
		// System.out.println("Header : "+temp.length());

		// Packing logic
	}
}

class Unpacker
{	
	public FileOutputStream outstream = null;

	public Unpacker(String src)
	{
		unpackFile(src);
	}

	public void unpackFile(String FilePath)
	{
		try
		{
			FileInputStream instream = new FileInputStream(FilePath);
			
			byte Header[] = new byte[100];
			int length = 0;
			int counter = 0;

			while((length = instream.read(Header,0,100)) > 0)
			{
				String str = new String(Header);
				
				// c:/asdas/asdasd/asdas/demo.txt 45
				String ext = str.substring(str.lastIndexOf("\\"));

				ext = ext.substring(1);

				String words[] = ext.split("\\s");
				String name = words[0];
				int size = Integer.parseInt(words[1]);

				byte arr[] = new byte[size];
				instream.read(arr,0,size);
				
				System.out.println("New File gets created as :"+name);
				// New file gets created
				FileOutputStream fout = new FileOutputStream(name);
				// Write the data into newnly created file
				fout.write(arr,0,size);

				counter++;
			}

			System.out.println("Sucessfully unpacked files : "+counter);
		}
		catch(Exception obj)
		{}
	}
}

class ClockLabel extends JLabel implements ActionListener
{
	String type;
	SimpleDateFormat sdf;

	public ClockLabel(String type)
	{
		this.type  = type;
		setForeground(Color.yellow);

		switch(type)
		{
			case "date" : sdf = new SimpleDateFormat(" MMMM dd yyyy");
				setFont(new Font("sans-serif",Font.PLAIN,12));
				setHorizontalAlignment(SwingConstants.LEFT);
				break;
			case "time" : sdf = new SimpleDateFormat("hh:mm:ss a");
				setFont(new Font("sans-serif",Font.PLAIN,40));
				setHorizontalAlignment(SwingConstants.CENTER);
				break;
			case "day" : sdf = new SimpleDateFormat("EEEE ");
				setFont(new Font("sans-serif",Font.PLAIN,16));
				setHorizontalAlignment(SwingConstants.RIGHT);
				break;
			default : sdf = new SimpleDateFormat();
				break; 
		}

		Timer t = new Timer(1000,this);
		t.start();
	}

	public void actionPerformed(ActionEvent ae)
	{
		Date d = new Date();
		setText(sdf.format(d));
	}
}

class Template extends JFrame implements Serializable , ActionListener
{
	JPanel _header;
	JPanel _content;
	JPanel _top;

	ClockLabel dayLabel;
	ClockLabel timeLabel;
	ClockLabel dateLabel;

	JButton minimize,exit;


	public Template()
	{
		setDefaultCloseOperation(javax.swing.WindowConstants.DISPOSE_ON_CLOSE);
		GridBagLayout grid = new GridBagLayout();
		setLayout(grid);

		_top = new JPanel();
		_top.setBackground(Color.LIGHT_GRAY);

		_top.setLayout(null);


		getContentPane().add(_top,new GridBagConstraints(0,0,1,1,1,5,GridBagConstraints.BASELINE,GridBagConstraints.BOTH,new Insets(0,0,0,0),0,0));
		_content = new JPanel();
		_content.setLayout(null);
		_content.setBackground(new Color(0,50,120));

	}	
} 





































































