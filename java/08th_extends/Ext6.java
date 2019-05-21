abstract class Father{
	private int money;

	public int getMoney(){return money;}
	public void setMoney(int money){
		this.money = money;
	}
	public abstract void study();
}


interface A{
	public static final int i = 10;
	public abstract int getNum();
}

interface B{
	public static String name = "InterfaceB";
	public abstract String getName();
}

	

class Son extends Father implements A,B{
	public int getNum(){return i;}
	public String getName(){return name;}
	
	public void study(){
		System.out.println("I am study");
	}
}


public class Ext6{
	public static void main(String args[]){
		Son son = new Son();
		System.out.println(son.getName());
		System.out.println(son.getNum());
		son.study();
		
	}
}
