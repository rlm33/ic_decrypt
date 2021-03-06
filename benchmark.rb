require 'benchmark'

$decrypter = "./decrypter"

$to_test = %w{alpha gamma light pulse zoned}

def ic_decrypt to_test, domain, numthreads=nil, mpi=false
	system("#{"mpirun -np #{numthreads} -machinefile hosts" if mpi} #{$decrypter} #{to_test} -d #{domain} #{"-p -n #{numthreads}" if numthreads} > log_benchmark.txt 2> err_log")
end

def benchmark to_test, domain_name = 'DOWNCASE', domain_size = 26
	puts "#{domain_name} for #{to_test}"
	Benchmark.bm do |x| 
		x.report("Simple:") { ic_decrypt(to_test, domain_name) }
		x.report("2 threads:") { ic_decrypt(to_test, domain_name, 2) }
		x.report("3 threads:") { ic_decrypt(to_test, domain_name, 3) }
		x.report("4 threads:") { ic_decrypt(to_test, domain_name, 4) }
		x.report("#{domain_size} threads:") { ic_decrypt(to_test, domain_name, domain_size) }
		x.report("MPI:") { ic_decrypt(to_test, domain_name, 10, true) }
	end
end

$to_test.each do |string|
	benchmark string, 'DOWNCASE', 26
end
