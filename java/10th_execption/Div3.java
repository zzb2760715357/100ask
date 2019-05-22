public class Div3{

	public static void main(String args[]){
		int m = Integer.parseInt(args[0]);
		int n = Integer.parseInt(args[1]);

		System.out.println("Begin of div");
		int r = div(m,n);
		System.out.println("end of div");

		System.out.println(m+"/"+n+"="+r);
	}

	public static int div(int m,int n) throws ArithmeticException{
		int r = 0;
		
		r = m / n ;

		return r;
	}
}
