class Outer{
	private int a = 10;
	class Inner{
		public void printInfo(){
			System.out.println("a = " + a);
		}
	}
}



public class InnerDemo{
	public static void main(String args[]){
		Outer o = new Outer();
		Outer.Inner i =  o.new Inner();
		i.printInfo();
	}
}
