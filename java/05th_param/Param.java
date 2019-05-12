public class Param{
	public static void main(String args[]){
		int x = 1;
		int p[] = new int[1];
		
		func(x);
		System.out.println(x);

		p[0] = 123;
		System.out.println("Before func2 " + p[0]);
		func2(p);
		System.out.println("After func2 " + p[0]);
		
		
	}

	public static void func(int x){
		x = 100;
	}

	public static void func2(int[] p){
		p[0] = 200;
	}
	
}
