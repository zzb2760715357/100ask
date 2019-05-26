package a;
import b.*;

class Mysubmath extends Mymath{
	void printInfo(){
		System.out.println("Mysubmath: x = " + x);
	}
}

public class Pack{
	public static void main(String args[]){
		Mysubmath m = new Mysubmath();
		//m.x = 0;
		m.printInfo();
	}
}
