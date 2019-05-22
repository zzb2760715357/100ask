public class Div2{

	public static void main(String args[]){
		int m = Integer.parseInt(args[0]);
		int n = Integer.parseInt(args[1]);

		System.out.println("Begin of div");
		int r = div(m,n);
		System.out.println("end of div");

		System.out.println(m+"/"+n+"="+r);
	}

	public static int div(int m,int n){
		int r = 0;
		try {
			r = m / n ;
		}catch (ArithmeticException e){
			System.out.println(e);
		}finally{
			System.out.println("This is finally of div");
		}

		return r;
	}
}
