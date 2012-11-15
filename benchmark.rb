require 'benchmark'

$decrypter = "./dist/Debug/GNU-Linux-x86/ic_decrypt"

$to_test = %w{alpha gamma light zoned}

def ic_decrypt to_test, domain, numthreads=nil
	system("#{$decrypter} #{to_test} -d #{domain} #{"-p -n #{numthreads}" if numthreads} > log_benchmark.txt")
end

def benchmark to_test, domain_name, domain_size
	puts "#{domain_name} for #{to_test}"
	Benchmark.bm do |x| 
		x.report("Simple:") { ic_decrypt(to_test, domain_name) }
		x.report("2 threads:") { ic_decrypt(to_test, domain_name, 2) }
		x.report("3 threads:") { ic_decrypt(to_test, domain_name, 3) }
		x.report("4 threads:") { ic_decrypt(to_test, domain_name, 4) }
		x.report("#{domain_size} threads:") { ic_decrypt(to_test, domain_name, domain_size) }
	end
end

$to_test.each do |string|
	benchmark string, 'DOWNCASE', 26
end