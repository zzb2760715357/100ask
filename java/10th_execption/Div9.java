public class Div9{

	public static void main(String args[]){
		int m = 0;
		int n = 0;
		int r = 0;

		System.out.println("Begin of div");
		try {
			m = Integer.parseInt(args[0]);
		    n = Integer.parseInt(args[1]);
			r = div(m,n);
		}catch (ArithmeticException e){
			System.out.println("main :" + e);
		}catch (NumberFormatException e){
			System.out.println("main :" + e);
		}catch (RuntimeException e){
			System.out.println("main :" + e);
		}
		System.out.println("end of div");

		System.out.println(m+"/"+n+"="+r);
	}

	//public static int div(int m,int n) throws ArithmeticException{
	public static int div(int m,int n){
		int r = 0;

		try{
			r = m / n ;
		}catch(ArithmeticException e){
			System.out.println("div :"+e);
			throw e;
		}finally{
			System.out.println("finally of div");
		}

		return r;
	}
}
