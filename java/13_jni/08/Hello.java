public class Hello{
	private String name;
	private int age;

	public static void main(String args[]){
		System.out.println("Hello World!");
	}

	public int sayhello_to(String name){
		System.out.println("java Hello , " + name + " I'm " + this.name + "," + age+"years old");
		return 123;
	}

	public static void sayhello_to(){
	
	}
	
}
