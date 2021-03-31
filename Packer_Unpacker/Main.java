import java.io.*;
import java.util.*;
import java.io.File;
import java.io.FileOutputStream;
import java.io.FileInputStream;
import java.math.BigInteger;
import java.security.MessageDigest;
import java.security.NoSuchAlgorithmException;

class Main
{
	public static void main(String arg[])
	{
		Scanner sobj = new Scanner(System.in);
		System.out.println("-------------------------------");
		System.out.println("Marvellous Packer - Unpacker");
	
		while(true)
		{
			System.out.println("-------------------------------");
			System.out.println("Enter your choice");
			System.out.println("1 : Packing");
			System.out.println("2 : Unpacking");
			System.out.println("3 : Exit");
			System.out.println("-------------------------------");
			String Dir,Filename;
			int choice = 0;
			choice = sobj.nextInt();

			switch(choice)
			{
				case 1:
					System.out.println("Enter Directory name");
					Dir = sobj.next();

					System.out.println("Enter the file name for packing");
					Filename = sobj.next();

					Packer pobj = new Packer(Dir,Filename);

				break;

				case 2:
					System.out.println("Enter packed file name");
					String name = sobj.next();
					Unpacker obj = new Unpacker(name);

					break;

				case 3:
						System.out.println("-------------------------------");
					System.out.println("Thank you for using Marvellous Packer Unpacker");
							System.out.println("-------------------------------");
					System.exit(0);
					break;

				default:
					System.out.println("Wrong choice");
					break;
			}
		}

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

class MD
{
	public String Md5(String FolderName,String FileName)
	{
		try
		{
			MessageDigest md = MessageDigest.getInstance("MD5");

			byte[] MessageDigest = md.digest(FileName.getBytes());

			BigInteger no = new BigInteger(1, MessageDigest);

			String hashtext = no.toString(16);

			while(hashtext.length() < 32)
			{
				hashtext = "0" + hashtext;
			}

			TravelDirectory(FolderName);

			return hashtext;
		}

		catch(NoSuchAlgorithmException e)
		{
			throw new RuntimeException(e);
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
				System.out.println("Dleted file : "+filename.getName());
				Md5(filename.getAbsolutePath());	
			}		
		}
		System.out.println("-------------------------------");
		System.out.println("Succesfully Dleted files : "+counter);
		System.out.println("-------------------------------");
	}

	public static void main(String args[]) throws NoSuchAlgorithmException
	{
		String s = "Pakshal Karande";
		System.out.println(Md5(s));
	}
}






























