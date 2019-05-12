class Persion {
	String name;
	String getName(){
		return "guangdong " + name;
	}

	public Persion(String n){
		name = n;
	}
}

public class Oop2{
	public static void main(String args[]){
		Persion p1 = new Persion("zhangsan");

		Persion p2 = new Persion("lisi");

		System.out.println(p1.getName());
		System.out.println(p2.getName());
	}
}
