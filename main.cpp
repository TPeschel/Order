//#include <QCoreApplication>

#include <iostream>
#include "order.hpp"

#include <QList>
#include <QVector>
#include <QString>

// just for laziness
#define prnt std::cout <<
#define cr   std::endl

typedef std::size_t Sz;

// a collection like list or vector of this class SomeDataSet should later be sorted by any of its member variables
// that means it should be able to be sorted by name, id or value like excel columns
struct SomeDataSet {

	// some name
	QString
	name;

	// some id
	int
	id;

	// some value
	double
	value;

	// default constructor
	// doesn't matter what values are in the variables
	// but is needed in QList, QVector, std::vector etc.
	SomeDataSet( ) {}

	// initialize constructor
	SomeDataSet( QString const & p_name, int p_id, double p_value ) :
	name( p_name ), id( p_id ), value( p_value ) {}

	// this function is needed for sorting by names and
	// gives true if the NAME in the left hand data set is smaller than the one of the right hand one
	static bool
	smallerName( SomeDataSet const & p_sc1, SomeDataSet const & p_sc2 ) {

		return p_sc1.name < p_sc2.name;
	}

	// this function is needed for sorting by ids and
	// gives true if the ID in the left hand data set is smaller than the one of the right hand one
	static bool
	smallerId( SomeDataSet const & p_sc1, SomeDataSet const & p_sc2 ) {

		return p_sc1.id < p_sc2.id;
	}

	// this function is needed for sorting by values and
	// gives true if the VALUE in the left hand data set is smaller than the one of the right hand one
	static bool
	smallerValue( SomeDataSet const & p_sc1, SomeDataSet const & p_sc2 ) {

		return p_sc1.value < p_sc2.value;
	}

	// make it printable
	QString
	toStr( ) const {

		return QString( "%1\tid: %2\tvalue: %3" ).arg( name ).arg( id ).arg( value );
	}
};


// get the minimum value of a vector
template< typename T >
T
min( std::vector< T > const & p_v ) {

	T
	m = p_v[ 0 ];

	for( Sz i = 1; i < p_v.size( ); ++ i ) {

		if( p_v[ i ] < m ) {

			m = p_v[ i ];
		}
	}

	return m;
}

// true if the minimum of the left vector is smaller than the minumum of right vector
template< typename T >
bool
smallerMin( std::vector< T > const & p_v1, std::vector< T > const & p_v2 ) {

	return min( p_v1 ) < min( p_v2 );
}

int
main( int argc, char * argv[ ] ) {

	// first sort a list of qstrings

	QList< QString >
	ls;

	ls << "Hans" << "Bert" << "Moni" << "Kurt" << "Fred" << "Frank" << "Susi" << "Fred" << "Peter" << "Alexander" << "Werner";

	prnt "unsorted names" << cr;

	for( int i = 0; i < ls.size( ); ++ i ) {

		prnt ls[ i ].toStdString( ) << cr;
	}

	// smaller< T >( ) is in qsort.hpp
	// for daily use one can and should use it
	// bool smaller< T >( T const & a, T const & b ) { return a < b; }

	// now we want to sort QStrings
	// so we can use smaller< QString >( )

	Order
	idx = order( ls, smaller< QString > );

	// idx now contains the sorted indices ( the order )
	// idx[ 0 ] is the index of the smallest element in the original list

	prnt cr << "sorted names" << cr;

	for( Sz i = 0; i < idx.size( ); ++ i ) {

		prnt ls[ idx[ i ] ].toStdString( ) << cr;
	}

	prnt cr << cr;



	// let's try to find the rank for a QVector of QString

	QVector< QString >
	vs;

	vs << "Hans" << "Bert" << "Moni" << "Kurt" << "Fred" << "Frank" << "Susi" << "Fred" << "Peter" << "Alexander" << "Werner";

	// again use smaller< QString >( )
	// but now compute the rank of the elements after computing their order

	idx = rank( vs, smaller< QString > );

	// idx now contains the rank of the names

	for( Sz i = 0; i < idx.size( ); ++ i ) {

		prnt "rank of name[ " << i << " ] = " << vs[ i ].toStdString( ) << " is " << idx[ i ] << "\n";
	}

	prnt cr << cr;



	// now want to sort a simple in array

	int
	a[ ] = { 7, 4, 5, 2, 1, 3, 6, 0 };

	// use smaller< int >
	// an array doesnt have a member function size()
	// so we have to tell qcksrt the size of the array

	idx = order( a, 8, smaller< int > );

	for( Sz i = 0; i < idx.size( ); ++ i ) {

		prnt a[ i ] << " => " << a[ idx[ i ] ] << cr;
	}

	prnt cr;



	// now comes the actual purpose of the whole game:
	// sorting a collection of some different typed values by any one of it
	// SomeCollection is defined at the beginning of this file
	QVector< SomeDataSet >
	someCollection;

	someCollection
		<< SomeDataSet( "Hansi",      5,  2.51 )
		<< SomeDataSet( "Berti",     15, -2.5 )
		<< SomeDataSet( "Alexander",  7,  12.5 )
		<< SomeDataSet( "Monika",     3, -12.255)
		<< SomeDataSet( "Tonibert", 849,  72.2 )
		<< SomeDataSet( "Froni",    192, +2312.25 )
		<< SomeDataSet( "Lukas",    255, -92.25 )
		<< SomeDataSet( "Felix",    104, -112.25 );

	// sort by name, id or value
	Order
	idxName  = order( someCollection, SomeDataSet::smallerName ),
	idxId    = order( someCollection, SomeDataSet::smallerId ),
	idxValue = order( someCollection, SomeDataSet::smallerValue );

	prnt cr << "original names" << cr;

	for( Sz i = 0; i < idxName.size( ); ++ i ) {

		prnt i << ": " << someCollection[ i ].toStr( ).toStdString( ) << cr;
	}

	prnt cr << "sorted by names" << cr;

	for( Sz i = 0; i < idxName.size( ); ++ i ) {

		prnt idxName[ i ] << ": " << someCollection[ idxName[ i ] ].toStr( ).toStdString( ) << cr;
	}

	prnt cr << "sorted by ids" << cr;

	for( Sz i = 0; i < idxId.size( ); ++ i ) {

		prnt idxId[ i ] << ": " << someCollection[ idxId[ i ] ].toStr( ).toStdString( ) << cr;
	}

	prnt cr << "sorted by values decreasing" << cr;

	for( int i = idxValue.size( ) - 1; 0 <= i; -- i ) {

		prnt idxValue[ i ] << ": " << someCollection[ idxValue[ i ] ].toStr( ).toStdString( ) << cr;
	}

	prnt cr << cr;



	// now let's sort the column vectors of a matrix by their minima

	prnt "sort rows of a matrix by their minima" << cr;

	prnt cr << "unsorted matrix" << cr;

	std::vector< std::vector< int > >
	matrix {
		{ 13, 23 },
		{ 33,  4 },
		{  1,  6 },
		{ 10,  9 },
		{ -7, 100 },
		{ 27, 10 }
	};

	for( Sz i = 0; i < matrix.size( ); ++ i ) {

		for( Sz j = 0; j < matrix[ i ].size( ); ++ j ) {

			prnt matrix[ i ][ j ] << "\t";
		}

		prnt cr;
	}

	idx = order( matrix, smallerMin< int > );

	prnt cr << "sorted matrix" << cr;

	for( Sz i = 0; i < idx.size( ); ++ i ) {

		for( Sz j = 0; j < matrix[ i ].size( ); ++ j ) {

			prnt matrix[ idx[ i ] ][ j ] << "\t";
		}

		prnt cr;
	}

	return 0;
}
