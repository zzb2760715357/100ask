class Persion {
	String name;
	String getName(){
		return "guangdong " + name;
	}
}

public class Oop{
	public static void main(String args[]){
		Persion p1 = new Persion();
		p1.name = "zhangsan";

		Persion p2 = new Persion();
		p2.name = "lisi";

		System.out.println(p1.getName());
		System.out.println(p2.getName());
	}
}
