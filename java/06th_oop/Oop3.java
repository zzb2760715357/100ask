class Persion {
	String name;
	int age;
	String getName(){
		return "guangdong " + name;
	}

	public Persion(){
		name = "null";
		age = 0;
	}

	public Persion(String name){
		this.name = name;
	}

	public Persion(String name,int age){
		this.name = name;
		this.age = age;
	}
}

public class Oop3{
	public static void main(String args[]){
		Persion p1 = new Persion("zhangsan");
		Persion p2 = new Persion("lisi");
		Persion p3 = new Persion();
		Persion p4 = new Persion("wangwu",6);

		System.out.println(p1.getName());
		System.out.println(p2.getName());
		System.out.println(p3.getName());
		System.out.println(p4.getName());
	}
}
