namespace UDlib.Cv {

	public class Solver {

		/// <summary>
		/// matrix decomposition types
		/// </summary>
		public enum DecompTypes {

			/// <summary>
			/// Gaussian elimination with the optimal pivot element chosen.
			/// </summary>
			DECOMP_LU = 0,
			/// <summary>
			/// singular value decomposition (SVD) method; the system can be over-defined and/or the
			/// matrix src1 can be singular
			/// </summary>
			DECOMP_SVD = 1,
			/// <summary>
			/// eigenvalue decomposition; the matrix src1 must be symmetrical
			/// </summary>
			DECOMP_EIG = 2,
			/// <summary>
			/// Cholesky \f$LL^T\f$ factorization; the matrix src1 must be symmetrical and
			/// positively defined
			/// </summary>
			DECOMP_CHOLESKY = 3,
			/// <summary>
			/// QR factorization; the system can be over-defined and/or the matrix src1 can be singular
			/// </summary>
			DECOMP_QR = 4,
			/// <summary>
			/// while all the previous flags are mutually exclusive, this flag can be used together
			/// with any of the previous; it means that the normal equations
			///
			///     \f$\texttt{src1}^T\cdot\texttt{src1}\cdot\texttt{dst}=\texttt{src1}^T\texttt{src2}\f$
			/// 
			/// are solved instead of the original system
			/// 
			///     \f$\texttt{src1}\cdot\texttt{dst}=\texttt{src2}\f$
			/// 
			/// </summary>
			DECOMP_NORMAL = 16

		};

		public static float SolveLinear (Matrix<float> matA, Matrix<float> vecB,
			Matrix<float> vecX, int flags) =>
			Plugin.solveLinear (matA, vecB, vecX, flags);
		
	}

}