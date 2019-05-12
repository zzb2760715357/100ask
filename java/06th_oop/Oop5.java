class Persion {
	static int count;
	String name;
	int age;
	String getName(){
		return "guangdong " + name;
	}

	static {
		System.out.println("static block");
	}

	{
		count ++;
		System.out.println("construct block");
	}

	public Persion(){
		System.out.println("construct method:Persion 1");
		name = "null";
		age = 0;
	}

	public Persion(String name){
		System.out.println("construct method:Persion 2");
		this.name = name;
	}

	public Persion(String name,int age){
		System.out.println("construct method:Persion 3");
		this.name = name;
		this.age = age;
	}

	static void printPersion(){
		System.out.println("This is a class of Persion");
	}
}

public class Oop5{
	public static void main(String args[]){
		Persion p1 = new Persion("zhangsan");
		Persion p2 = new Persion("lisi");
		Persion p3 = new Persion();
		Persion p4 = new Persion("wangwu",6);

		System.out.println(p1.getName());
		System.out.println(p2.getName());
		System.out.println(p3.getName());
		System.out.println(p4.getName());

		Persion.printPersion();
		System.out.println(Persion.count);
	}
}
